# Automata-Powered Syntax Highlighter (`engine`)

This project is a C++ tool that uses **finite automata and regular expressions** to highlight text/code based on configurable regex groups. It outputs a single HTML file (`output.html`) that contains:

- **Legend** (colors + what they mean)
- **Highlighted text**
- **Output Log** (simple errors/warnings like missing semicolons, bracket issues, suggestions)

## Features

- **Config-driven highlighting**: reads one or more `.ini` files defining groups (regex + style).
- **Automata pipeline**: regex → ENFA → DFA → minimized DFA.
- **Basic checks**: bracket balancing + missing semicolons (for non-control lines).
- **Suggestions**: detects “close to correct” tokens for certain groups.
- **Single-file output**: writes `output.html` to the **working directory**.

## Quickstart (CLion)

The program uses **relative paths** at runtime. The easiest way to run it is to set the working directory to the `files` folder.

- **Target**: `engine`
- **Working directory**: `<repo>\files`
- **Program arguments** (pick one approach):
  - **No args**: `engine` will read `filelist.txt` from the working directory (one `.ini` path per line).
  - **With args**: pass one or more `.ini` files directly (e.g. `configuratiebestand.ini new_configuratiebestand.ini`).

When you run:

- **Input**: `files\input.txt`
- **Output**: `files\output.html`

## Run

### Build (from repo root)

```bat
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Run (set working directory to `files`)

#### Step 1 — move to the input directory

```bat
cd files
```

#### Step 2 — run the program
With explicit config files:

```bat
..\build\Release\engine.exe configuratiebestand.ini new_configuratiebestand.ini
```

#### Or with `filelist.txt`:

```bat
..\build\Release\engine.exe
```


## Configuration (`.ini`) format

Config files live in `files/` (examples: `configuratiebestand.ini`, `new_configuratiebestand.ini`).

- **Per-group sections**: `[groep0]`, `[groep1]`, ...
- **Keys**:
  - **RE**: regex/pattern string
  - **color**: hex color (e.g. `#d73a49`)
  - **fontWeight**: numeric font weight (e.g. `600`)
  - **displayName**: label shown in the Legend
- **General section**:
  - **NumberOfGroups**: must match the number of `[groepX]` sections

Configuration Format (.ini)

Configuration files define regex groups used for highlighting.

## Example structure:

```ini
[General]
NumberOfGroups=2
[groep0]
RE=\b(int|float|if|else|for|while)\b
color=#d73a49
fontWeight=600
displayName=Keywords
[groep1]
RE=//.*
color=#6a737d
fontWeight=400
displayName=Comments
```


## Output (`output.html`) structure

Open `files\output.html` in a browser. It has three main parts:

- **Legend (left sidebar)**: each configured group + its color/name.
- **Highlighted text (main area)**: the contents of `input.txt`, tokenized and styled when a group matches.
- **Output Log (bottom panel)**: errors/warnings/suggestions collected during processing.

## Example screenshots:

![Legend](docs/legend.png)
![Highlighted Text](docs/highlighted-text.png)
![Output Log](docs/output-log.png)