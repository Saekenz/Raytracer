Tested environments:
    DEV-Computer: Microsoft Windows 10 Home (Version: 10.0.19045 Build 19045)
    Visual Studio Code 1.78.2

Additional and general remarks:
    To help with parsing the .xml files the RapidXML library was used (rapidxml.hpp in util folder).
    The xmlparser class automatically changes the output file name to .ppm (if .png is supplied) to prevent file type errors.
    The output after rendering a scene is always a file with the .ppm format.
    To open the .ppm files the PPM Viewer for Visual Studio Code extension was used.
    
    Command to compile the program:
        g++ Raytracer/main.cpp Raytracer/xmlparser.cpp Raytracer/raytracer.cpp Raytracer/scene.cpp Raytracer/camera.cpp -o main

    or (if already in Raytracer directory):
        g++ main.cpp xmlparser.cpp raytracer.cpp scene.cpp camera.cpp -o main

    Command to render a scene:
        ./main <input_file_path>
    
    Example: ./main Raytracer/scenes/example1.xml
    (Renders the scene described in example1.xml located in folder Raytracer/scenes)
