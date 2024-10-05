# Julia Set Visualizer

The **Julia Set Visualizer** is a high-performance tool for exploring Julia set fractals, using CUDA for parallel computation and SDL2 for rendering. It provides an interactive experience to navigate and manipulate fractal visuals smoothly. 

## Features

- **CUDA Acceleration:** Parallelized computation for fast and smooth fractal rendering.
- **Interactive Controls:** 
  - **Click & Drag:** Move around the fractal space.
  - **Arrow Keys:** Adjust the constant `C` to explore various fractal shapes.
  - **Zooming:** Use `+` and `-` keys to zoom in and out.
- **SDL2 Rendering:** Real-time graphical output for responsive fractal exploration.


## Installation

### Prerequisites

- CUDA-compatible GPU and Toolkit
- SDL2 library
- C++ compiler with CUDA support

### Steps

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/julia-set-visualizer.git
   cd julia-set-visualizer
   ```

2. Install SDL2:
   - On Ubuntu: `sudo apt-get install libsdl2-dev`
   - On macOS: `brew install sdl2`
   - On Windows: Follow the instructions on the [SDL2 website](https://www.libsdl.org/download-2.0.php).

3. Build the project:
   ```
   make
   ```

4. Run the application:
   ```
   ./julia_set_cuda
   ```

## Usage

- **Click & Drag:** Move within the fractal.
- **Arrow Keys:** Change the constant `C` to explore different shapes.
- **Zoom:** `+` to zoom in, `-` to zoom out.


## Performance Tips

- Update your GPU drivers and CUDA toolkit for the best performance.
- Experiment with CUDA kernel configurations for optimal results on your hardware.

## Contributing

Contributions are welcome! Fork the repository and submit pull requests for code improvements, new features, or documentation enhancements.

## License

Licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgements

- [SDL2](https://www.libsdl.org/) for graphical rendering.
- NVIDIA's CUDA Toolkit for parallel computing.

---

Enjoy exploring the world of Julia fractals! 

