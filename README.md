
# Interactive Solar System Visualization using OpenGL/GLUT

An interactive 3D visualization of our solar system implemented using OpenGL and GLUT. The application features a dynamic view of the solar system with planets orbiting around the sun, complete with interactive camera controls and visualization options.

## Features

- Realistic orbital motion of all 8 planets (Mercury through Neptune)
- Interactive camera controls for viewing the solar system from different angles
- Two visualization modes:
  - Dynamic orbital view with rotating planets
  - Educational static view with planet descriptions
- Planet customization through a right-click menu
- Gradient space background with stars
- Realistic planet size ratios and orbital paths

## Controls

### Mouse Controls
- **Left Click**: Start/Stop planet revolution
- **Right Click**: Toggle between orbital view and educational view
- **Middle Click**: Opens menu to customize planet colors

### Keyboard Controls
- **F**: Move camera forward
- **B**: Move camera backward
- **L**: Move camera left
- **R**: Move camera right
- **U**: Move camera up
- **D**: Move camera down
- **S**: Reset camera to default position
- **ESC**: Exit application

## Technical Features

- 3D rendering using OpenGL/GLUT
- Dynamic lighting system
- Elliptical orbital paths
- Gradient background with procedurally generated stars
- Smooth animation system
- Color customization for planets
- Text rendering for educational information

## Dependencies

- OpenGL
- GLUT (OpenGL Utility Toolkit)
- C++ compiler with C++11 support

## Building and Running

1. Make sure you have OpenGL and GLUT installed on your system
2. Compile the source code using a C++ compiler:
   ```bash
   g++ -o solarsystem Source.cpp -lglut -lGL -lGLU
   ```
3. Run the executable:
   ```bash
   ./solarsystem
   ```

## Educational Content

The application includes an educational view (accessed via right-click) that displays detailed information about each celestial body, including:
- The Sun and its characteristics
- All eight planets with their unique features
- Size comparisons
- Orbital characteristics

## Performance Notes

- The application is optimized for 1920x1080 resolution
- Smooth animations are maintained through efficient rendering techniques
- Memory usage is optimized for long-running sessions

## Future Improvements

- Addition of planet moons
- Texture mapping for more realistic planet appearances
- Additional astronomical objects (asteroids, comets)
- Enhanced lighting effects
- User-configurable orbital speeds

## License

This project is open-source and available under the MIT License. Feel free to use, modify, and distribute as needed.
