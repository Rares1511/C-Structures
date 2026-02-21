# Context
Analyze the current workspace (@workspace), focusing on `.h` and `.c` files to understand the data structures, memory management, and API surface.

# Instructions
Please generate a README with the following sections, optimized for GitHub rendering:

## 1. Project Header & Visuals
- A clear, bold Title.
- (Optional) Placeholder for build status badges.
- A concise "TL;DR" of what the project solves.

## 2. API Reference (Detailed)
For every public function found in the headers, create a detailed entry. Use a GitHub-flavored Markdown table or a list with the following:
- **Function**: `name()`
- **Return Value**: Detailed explanation of return types (e.g., `0` for success, `-1` for `errno`, or specific pointers).
- **Description**: A short, clear explanation of the internal logic and any side effects (e.g., "allocates memory that must be freed by the caller").

## 3. Memory Architecture & Analysis
This section must be mathematically rigorous. 
- **Structure Breakdown**: Provide the `sizeof` for primary structures. Account for 64-bit alignment and padding.
- **Memory Growth Formulas**: Use LaTeX for all growth equations so they render via GitHub's MathJax support.
  - *Example*: Total memory $M$ for $n$ elements in a dynamic array: 
    $$M(n) = S_{header} + (capacity \times S_{element})$$
- **Scaling Behavior**: Describe the Big O complexity of memory usage (e.g., $O(n)$) and how the allocator behaves during additions or subtractions (e.g., "shrinks by half when occupancy falls below 25%").

## 4. GitHub Compatibility & UX
- **Table of Contents**: Include a clickable "Table of Contents" at the top for easy navigation.
- **Code Blocks**: Ensure all blocks use ` ```c ` for syntax highlighting.
- **Admonitions**: Use GitHub's "Note" or "Warning" callouts (e.g., `> [!NOTE]`) for important memory management tips.

## 5. Build & Usage
- Clear instructions using `make` or `gcc`.
- A "Quick Start" code snippet showing the library in action.

# Style Guidelines
- Use a professional, peer-to-peer technical tone.
- Avoid fluff; prioritize technical accuracy and scannability.