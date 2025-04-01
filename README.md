# BST Box - Binary Search Tree console visualization

## Why It Was Developed

```
                     ┏━━━┓                     
           ┏━━━━━━━━━┫ 5 ┣━━━━━━━━━┓           
           ┃         ┗━━━┛         ┃           
         ┏━┻━┓                   ┏━┻━┓         
     ┏━━━┫ 2 ┣━━━┓           ┏━━━┫11 ┣━━━┓     
     ┃   ┗━━━┛   ┃           ┃   ┗━━━┛   ┃     
   ┏━┻━┓       ┏━┻━┓       ┏━┻━┓       ┏━┻━┓   
   ┃ 1 ┃       ┃ 3 ┃       ┃ 7 ┃       ┃13 ┃   
   ┗━━━┛       ┗━━━┛       ┗━━━┛       ┗━━━┛
```

The idea draws inspiration from visualizer apps that present programming concepts in
an intuitive and engaging way. With this goal in mind, this project was developed to
visualize Binary Tree on console UI with text-based presentation. Under text form, Binary Tree can be shared straightforwardly in text communication like technical documents, code reviews, or source code documentation, all without the need of images.

Currently the program demonstrates how AVL Tree changes through out insertion and deletion of nodes. The output is made with Unicode characters, and can be exported to a text file for sharing.

## Processing Flow

Below is an abstract sequence of how the application operates:

```
+-----------------------+
| Command Line Interface|
+-----------------------+
            |
            v
+-----------------------+
|    Input Processing   |
+-----------------------+
            |
            v
+-----------------------+
| Binary Search Tree    |
| Implementation        |
+-----------------------+
            |
            v
+-----------------------+
|     Tree Drawing      |
+-----------------------+
            |
            v
+-----------------------+
|     Console Output    |
+-----------------------+
```

## How to Compile and Run

1. **Compile the Code**:
   ```bash
   ./scripts/build.sh
   ```
   Output executable: `build/visualizer/Visualizer`

1. **Run the Application**:
   ```bash
   ./scripts/run.sh
   ```
   Log file: `build/visualizer/TreePresenterLogs.log`

1. **Test the Core Logic**:
   ```bash
   ./scripts/test.sh
   ```
   Log file: `build/treeview/test/TreeViewTestLogs.log`

## Project Structure

The project is organized as follows:

```
root/
├── treeview/     # Binary tree implementation 
│                 # and output manipulation
│                 # - CMake Library
│
├── visualizer/   # Main application interface
│                 # - CMake Executable
│
├── build/        # CMake build output directory
│
└── scripts/      # Scripts for one-line commands                 
                  # to compile and run the app

```

## Technologies Used

| **Technology**         | **Description**                                   |
|-------------------------|---------------------------------------------------|
| **Programming Language**| C++.                                              |
| **Build System**        | CMake for managing modules and build process                |
| **Testing Framework**   | Google Test for unit testing.                     |
| **Logging**       | spdlog for debugging                                           |
| **Scripting**   | Bash scripts for compile and execution           |
| **Version Control**     | Git for source code management                   |

## Future Enhancements
- **Visualization**: Show step-by-step of tree changes for each insertion and deletion.
- **Tree Types**: Implement different Self-Balacing Binary Tree types.
- **Text-based presentation**: Non-monospaced font types.
- **Export Functionality**: Export to different content types (now only text file supported).
- **Import Functionality**: Allow reading tree content from file.