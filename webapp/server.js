const express = require("express");
const fs = require("fs");
const path = require("path");

const app = express();
const port = process.env.PORT || 3000;

const dataDir = path.join(__dirname, "..", "data");
const locationsPath = path.join(dataDir, "locations.txt");
const edgesPath = path.join(dataDir, "edges.txt");
const permanentMapPath = path.join(__dirname, "..", "src", "map.jpeg");

const locationCoords = {
  "APJ lecture hall": [0.50, 0.26],
  "Student Centre": [0.48, 0.31],
  "Student Activity Centre": [0.55, 0.20],
  Library: [0.69, 0.56],
  "Block 6": [0.50, 0.50],
  "Block 5": [0.53, 0.77],
  "Block 8A": [0.56, 0.71],
  "Block 4": [0.41, 0.77],
  "Admin Block": [0.35, 0.69],
  Nescafe: [0.24, 0.63],
  "Sports Complex": [0.86, 0.83],
  "Boys Hostel": [0.24, 0.21],
  "Girls Hostel": [0.63, 0.91],
  "Department of Design": [0.21, 0.10],
  TnP: [0.70, 0.80],
  "Guest House": [0.68, 0.13],
};

function readLocations() {
  return fs
    .readFileSync(locationsPath, "utf8")
    .split(/\r?\n/)
    .map((line) => line.trim())
    .filter(Boolean);
}

function readEdges() {
  return fs
    .readFileSync(edgesPath, "utf8")
    .split(/\r?\n/)
    .map((line) => line.trim())
    .filter(Boolean)
    .map((line) => line.split(",").map((x) => x.trim()))
    .filter((pair) => pair.length === 2);
}

function buildGraph() {
  const locations = readLocations();
  const edges = readEdges();
  const indexByName = new Map(locations.map((name, idx) => [name, idx]));
  const adjacency = Array.from({ length: locations.length }, () => []);

  for (const [from, to] of edges) {
    const a = indexByName.get(from);
    const b = indexByName.get(to);
    if (a === undefined || b === undefined) {
      continue;
    }
    adjacency[a].push(b);
    adjacency[b].push(a);
  }

  return { locations, edges, adjacency };
}

function distanceBetween(a, b) {
  const pointA = locationCoords[a];
  const pointB = locationCoords[b];

  if (!pointA || !pointB) {
    return 1;
  }

  const dx = pointA[0] - pointB[0];
  const dy = pointA[1] - pointB[1];
  return Math.sqrt(dx * dx + dy * dy);
}

function shortestPath(from, to) {
  const { locations, adjacency } = buildGraph();
  const indexByName = new Map(locations.map((name, idx) => [name, idx]));
  const start = indexByName.get(from);
  const end = indexByName.get(to);

  if (start === undefined || end === undefined) {
    return null;
  }

  const distances = Array(locations.length).fill(Infinity);
  const visited = Array(locations.length).fill(false);
  const parent = Array(locations.length).fill(-1);
  distances[start] = 0;

  for (let count = 0; count < locations.length; count++) {
    let node = -1;
    let bestDistance = Infinity;

    for (let i = 0; i < locations.length; i++) {
      if (!visited[i] && distances[i] < bestDistance) {
        bestDistance = distances[i];
        node = i;
      }
    }

    if (node === -1) {
      break;
    }

    visited[node] = true;

    for (const next of adjacency[node]) {
      const edgeWeight = distanceBetween(locations[node], locations[next]);
      const newDistance = distances[node] + edgeWeight;

      if (newDistance < distances[next]) {
        distances[next] = newDistance;
        parent[next] = node;
      }
    }
  }

  if (!isFinite(distances[end])) {
    return [];
  }

  const pathIndices = [];
  for (let cur = end; cur !== -1; cur = parent[cur]) {
    pathIndices.push(cur);
  }
  pathIndices.reverse();

  return pathIndices.map((idx) => locations[idx]);
}

app.use(express.static(path.join(__dirname, "public")));

app.get("/map.jpeg", (_req, res) => {
  if (!fs.existsSync(permanentMapPath)) {
    res.status(404).json({ error: "Map image not found at src/map.jpeg" });
    return;
  }

  res.sendFile(permanentMapPath);
});

app.get("/api/locations", (_req, res) => {
  res.json(readLocations());
});

app.get("/api/graph", (_req, res) => {
  const { locations, edges } = buildGraph();
  res.json({ locations, edges });
});

app.get("/api/shortest-path", (req, res) => {
  const from = (req.query.from || "").toString().trim();
  const to = (req.query.to || "").toString().trim();

  if (!from || !to) {
    res.status(400).json({ error: "Query params 'from' and 'to' are required." });
    return;
  }

  const result = shortestPath(from, to);
  if (result === null) {
    res.status(404).json({ error: "Location not found." });
    return;
  }

  res.json({ from, to, path: result, hops: Math.max(0, result.length - 1) });
});

app.listen(port, () => {
  console.log(`NSUT maps running at http://localhost:${port}`);
});
