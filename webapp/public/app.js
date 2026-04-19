const fromSelect = document.getElementById("fromLocation");
const toSelect = document.getElementById("toLocation");
const findBtn = document.getElementById("findBtn");
const statusEl = document.getElementById("status");
const hopCountEl = document.getElementById("hopCount");
const pathSummaryEl = document.getElementById("pathSummary");
const canvas = document.getElementById("routeCanvas");
const ctx = canvas.getContext("2d");

const BASE_WIDTH = 768;
const BASE_HEIGHT = 1183;

const LOCATION_COORDS = {
  "APJ lecture hall": [0.50, 0.26],
  "Student Centre": [0.48, 0.31],
  "Student Activity Centre": [0.55, 0.20],
  "Library": [0.69, 0.56],
  "Block 6": [0.50, 0.50],
  "Block 5": [0.53, 0.77],
  "Block 8A": [0.56, 0.71],
  "Block 4": [0.41, 0.77],
  "Admin Block": [0.35, 0.69],
  "Nescafe": [0.24, 0.63],
  "Sports Complex": [0.86, 0.83],
  "Boys Hostel": [0.24, 0.21],
  "Girls Hostel": [0.63, 0.91],
  "Department of Design": [0.21, 0.10],
  "TnP": [0.70, 0.80],
  "Guest House": [0.68, 0.13],
};

let graphData = { locations: [], edges: [] };
let currentPath = [];
let mapImage = null;

async function fetchJSON(url) {
  const res = await fetch(url);
  if (!res.ok) {
    throw new Error(`Request failed (${res.status})`);
  }
  return res.json();
}

function fillSelect(select, values) {
  select.innerHTML = "";
  values.forEach((name) => {
    const option = document.createElement("option");
    option.value = name;
    option.textContent = name;
    select.appendChild(option);
  });
}

function getPoint(name) {
  const pair = LOCATION_COORDS[name];
  if (!pair) {
    return null;
  }
  return {
    x: pair[0] * canvas.width,
    y: pair[1] * canvas.height,
  };
}

function resizeCanvas() {
  const dpr = window.devicePixelRatio || 1;
  const width = Math.min(820, canvas.parentElement.clientWidth - 10);
  const height = Math.round(width * (BASE_HEIGHT / BASE_WIDTH));

  canvas.style.width = `${width}px`;
  canvas.style.height = `${height}px`;
  canvas.width = Math.round(width * dpr);
  canvas.height = Math.round(height * dpr);

  ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
  drawMap();
}

function drawBaseLayer() {
  const width = canvas.clientWidth;
  const height = canvas.clientHeight;

  ctx.clearRect(0, 0, width, height);

  if (mapImage) {
    ctx.drawImage(mapImage, 0, 0, width, height);
  } else {
    const grad = ctx.createLinearGradient(0, 0, width, height);
    grad.addColorStop(0, "#f7f0df");
    grad.addColorStop(1, "#dbefe9");
    ctx.fillStyle = grad;
    ctx.fillRect(0, 0, width, height);

    ctx.strokeStyle = "rgba(34, 56, 64, 0.10)";
    ctx.lineWidth = 1;
    for (let x = 20; x < width; x += 36) {
      ctx.beginPath();
      ctx.moveTo(x, 0);
      ctx.lineTo(x, height);
      ctx.stroke();
    }
    for (let y = 20; y < height; y += 36) {
      ctx.beginPath();
      ctx.moveTo(0, y);
      ctx.lineTo(width, y);
      ctx.stroke();
    }
  }
}

function drawHighlightedPath() {
  if (!currentPath || currentPath.length < 2) {
    return;
  }

  ctx.lineCap = "round";
  ctx.lineJoin = "round";
  ctx.strokeStyle = "#ff4d2e";
  ctx.lineWidth = 8;
  ctx.shadowColor = "rgba(255, 77, 46, 0.75)";
  ctx.shadowBlur = 22;

  for (let i = 0; i < currentPath.length - 1; i++) {
    const p1 = getPoint(currentPath[i]);
    const p2 = getPoint(currentPath[i + 1]);
    if (!p1 || !p2) {
      continue;
    }

    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.stroke();
  }

  ctx.shadowBlur = 0;
  ctx.globalAlpha = 0.3;
  ctx.strokeStyle = "#ffd36b";
  ctx.lineWidth = 13;
  for (let i = 0; i < currentPath.length - 1; i++) {
    const p1 = getPoint(currentPath[i]);
    const p2 = getPoint(currentPath[i + 1]);
    if (!p1 || !p2) {
      continue;
    }

    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.stroke();
  }
  ctx.globalAlpha = 1;
}

function drawNodes() {
  if (!currentPath || currentPath.length === 0) {
    return;
  }

  currentPath.forEach((name, idx) => {
    const p = getPoint(name);
    if (!p) {
      return;
    }

    const isStart = idx === 0;
    const isEnd = idx === currentPath.length - 1;
    ctx.save();
    ctx.shadowColor = isStart ? "rgba(15, 122, 115, 0.95)" : isEnd ? "rgba(33, 73, 139, 0.95)" : "rgba(255, 77, 46, 0.9)";
    ctx.shadowBlur = 18;
    ctx.fillStyle = isStart ? "#0f7a73" : isEnd ? "#21498b" : "#ff4d2e";
    ctx.beginPath();
    ctx.arc(p.x, p.y, isStart || isEnd ? 8 : 6, 0, Math.PI * 2);
    ctx.fill();
    ctx.restore();

    ctx.save();
    ctx.strokeStyle = "rgba(255, 255, 255, 0.9)";
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(p.x, p.y, isStart || isEnd ? 10 : 8, 0, Math.PI * 2);
    ctx.stroke();
    ctx.restore();

    ctx.fillStyle = "#102c2a";
    ctx.font = "600 12px 'IBM Plex Mono', monospace";
    ctx.fillText(name, p.x + 8, p.y - 8);
  });
}

function drawMap() {
  drawBaseLayer();
  drawHighlightedPath();
  drawNodes();
}

function setPathSummary(path) {
  if (!path || path.length === 0) {
    pathSummaryEl.textContent = "No route available for selected locations.";
    hopCountEl.textContent = "";
    return;
  }

  pathSummaryEl.textContent = `${path[0]} -> ${path[path.length - 1]}`;
  hopCountEl.textContent = `Hops: ${Math.max(0, path.length - 1)}`;
}

function loadPermanentMap() {
  return new Promise((resolve) => {
  const img = new Image();
  img.onload = () => {
    mapImage = img;
      resolve(true);
  };
  img.onerror = () => {
      resolve(false);
  };
    img.src = "/map.jpeg";
  });
}

findBtn.addEventListener("click", async () => {
  const from = fromSelect.value;
  const to = toSelect.value;

  if (!from || !to) {
    statusEl.textContent = "Choose both locations.";
    return;
  }

  statusEl.textContent = "Finding route...";
  try {
    const data = await fetchJSON(`/api/shortest-path?from=${encodeURIComponent(from)}&to=${encodeURIComponent(to)}`);
    currentPath = data.path || [];
    setPathSummary(currentPath);
    drawMap();
    statusEl.textContent = "Pictorial route rendered.";
  } catch (err) {
    currentPath = [];
    setPathSummary(currentPath);
    drawMap();
    statusEl.textContent = `Could not compute route: ${err.message}`;
  }
});

async function init() {
  statusEl.textContent = "Loading campus graph...";

  try {
    const [locations, graph, mapLoaded] = await Promise.all([
      fetchJSON("/api/locations"),
      fetchJSON("/api/graph"),
      loadPermanentMap(),
    ]);

    graphData = graph;
    fillSelect(fromSelect, locations);
    fillSelect(toSelect, locations);

    if (locations.length > 1) {
      toSelect.selectedIndex = 1;
    }

    resizeCanvas();
    statusEl.textContent = mapLoaded
      ? "Ready. Shortest route will be drawn on NSUT map."
      : "Ready. Map image missing at src/map.jpeg, using fallback background.";
  } catch (err) {
    statusEl.textContent = `Error: ${err.message}`;
  }
}

window.addEventListener("resize", resizeCanvas);
init();
