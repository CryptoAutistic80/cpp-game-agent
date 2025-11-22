# cpp-game-agent

A lightweight C++ playground for experimenting with persona-driven agents that call tools via the Groq Responses API. The project ships with a basic CLI example, reusable headers, and JSON-based persona/behaviour profiles.

## Purpose
- Explore a minimal end-to-end flow for prompting Groq's Responses API from C++.
- Provide reusable building blocks (requests, responses, persona config) for new agent behaviors.
- Offer a reference CLI that demonstrates environment loading, persona selection, and tool-call parsing.

## Features
- Persona-driven prompts that combine character, behaviour, and memory JSON assets.
- Groq Responses API client built on `libcurl` and `nlohmann::json`.
- CLI demo that loads personas from `characters/` and performs a single turn with optional tool-calls.
- Simple extension points for adding new tools and custom memory handling.

## Project layout
- `include/`: public headers for building agents, requests, and responses.
- `src/`: Groq backend implementation and agent orchestration logic.
- `examples/`: CLI demo wiring persona + behaviour + memory to a Groq request.
- `characters/`: sample JSON files for personas and behaviour tuning.

## Groq Responses API usage
- **Base URL:** `https://api.groq.com/openai/v1/responses`.
- **Authentication:** set the `GROQ_API_KEY` environment variable to your Groq API key before running. Requests without a key will fail against the live service.
- **Behavior:** sends text prompts and decodes any `tool_calls` array returned in the first choice message; unexpected shapes are ignored gracefully.
- **Supported models:** examples use Groq's compatible models such as `llama3-groq-8b-8192-tool-use-preview` and `mixtral-8x7b-32768` (update as new models become available).

## Building
Prerequisites: CMake (3.16+), a C++17 compiler, `libcurl` development headers, and internet access for FetchContent to download `nlohmann::json`.

```bash
cmake -S . -B build
cmake --build build
```

## CLI usage
Run the CLI example after building:

```bash
export GROQ_API_KEY="<your key>"
./build/agent_cli \
  --character ./characters/default/character.json \
  --behaviour ./characters/default/behaviour.json \
  --memory ./characters/default/memory.json
```

## Planned roadmap
- Add richer tool-call examples (file system, retrieval, or game actions).
- Introduce streaming output support for long responses.
- Provide tests for persona parsing and response handling.
- Package examples as installable binaries.

## Contribution guidelines
- Open an issue describing the change before submitting a PR.
- Keep additions minimal and well-documented; prefer small, focused commits.
- Follow existing code style and avoid introducing new dependencies without discussion.
- Include build or usage notes in the PR description when applicable.

## Licensing
This repository is distributed under the MIT License. See [LICENSE](LICENSE) for details.
