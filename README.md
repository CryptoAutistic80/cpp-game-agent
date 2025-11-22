# cpp-game-agent

A lightweight C++ playground for experimenting with persona-driven agents that call tools via the Groq Responses API. The project ships with a basic CLI example, reusable headers, and JSON-based persona/behaviour profiles.

## Project layout
- `include/`: public headers for building agents, requests, and responses.
- `src/`: Groq backend implementation and agent orchestration logic.
- `examples/`: CLI demo wiring persona + behaviour + memory to a Groq request.
- `characters/`: sample JSON files for personas and behaviour tuning.

## Building
This project uses CMake and FetchContent for `nlohmann::json` while linking against the system `libcurl`.

```bash
cmake -S . -B build
cmake --build build
```

To run the CLI example after building:
```bash
export GROQ_API_KEY="<your key>"
./build/agent_cli
```

## Groq Responses API notes
- The Responses API is currently in beta; payloads and response shapes may evolve.
- This sample targets `https://api.groq.com/openai/v1/responses` and uses a text-only prompt for simplicity.
- Tool-calls are decoded from any `tool_calls` array returned in the first choice message; unexpected shapes are ignored gracefully.
- Set the `GROQ_API_KEY` environment variable before running the CLI. Requests without a key will fail against the live service.

## Licensing
This repository is distributed under the MIT License. See [LICENSE](LICENSE) for details.
