# NSUT Campus DSA Application

This project models NSUT campus locations and their connections using a graph data structure.

It includes:
- A native C CLI app
- C-based tests
- A minimalist web app (frontend + API backend)

## Project Structure

```
nsut-campus-dsa-app
├── src
│   ├── main.c
│   ├── graph.c
│   ├── queue.c
│   ├── stack.c
│   ├── shortest_path.c
│   └── utils.c
├── include
│   ├── graph.h
│   ├── queue.h
│   ├── stack.h
│   ├── shortest_path.h
│   └── utils.h
├── data
│   ├── locations.txt
│   └── edges.txt
├── tests
│   └── test_graph.c
├── webapp
│   ├── server.js
│   ├── package.json
│   └── public
│       ├── index.html
│       ├── styles.css
│       └── app.js
├── Makefile
└── README.md
```

## Prerequisites

- GCC (MinGW on Windows)
- `mingw32-make`
- Node.js (for web app)

## C CLI Build and Run (Windows)

From the project root:

```powershell
$env:Path += ";C:\MinGW\bin"
mingw32-make
.\nsut-campus-dsa-app.exe
```

## C Tests

```powershell
mingw32-make test
```

## Web App

From the `webapp` directory:

```powershell
npm install
npm start
```

Then open `http://localhost:3000` in your browser.

## Notes

- Data files are read from `data/locations.txt` and `data/edges.txt`.
- Shortest path is computed using BFS on an unweighted graph.
