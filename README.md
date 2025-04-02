# Binary Search Tree Console Visualization

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
visualize Binary Tree on console UI in text-based presentation. Under text form, Binary Tree can be shared straightforwardly in text communication like technical documents, code reviews, or source code documentation, all without the need of images.

Currently the program demonstrates how AVL Tree changes through out insertion and deletion. The output is made with Unicode characters, and can be exported to a text file for sharing.

## Project Structure

The project is organized as follows:

```
root/
│
├── bstbox/       # Main application interface
│
├── treeview/     # Binary tree implementation 
│                 # and output manipulation
│
├── build/        # Build output directory
│
└── scripts/      # Scripts for one-line commands                 
                  # to compile and run the app

```


## Architecture Diagram
```
╭┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╮
┆                      Main program module  ┆
┆       ┏━━━━━━━━━━━━━━━━━━━┓               ┆ 
┆    ┏━━┫ Program Interface ┣━━┓            ┆
┆    ┃  ┗━━━━━━━━━━━━━━━━━━━┛  ┃            ┆
┆┄┄┄┄┃┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┃┄┄┄┄┄┄┄┄┄┄┄┄┆
┆ ┏━━┻━━┓               ┏━━━━━━┻━━━━━━┓     ┆
┆ ┃ BST ┃               ┃ BST Drawing ┃     ┆
┆ ┗━━━━━┛               ┗━━━━━━━━━━━━━┛     ┆
┆                                           ┆
┆               Tree implementation module  ┆ 
╰┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╯
```

## How to Compile and Run
   
1. **Compile the Code**:
   ```bash
   ./scripts/build.sh
   ```
   Output executable: `build/bstbox/BSTBox`

1. **Run the Application**:
   ```bash
   ./scripts/run.sh
   ```
   Log file: `build/bstbox/BSTBoxLogs.log`

1. **Test the Core Logic**:
   ```bash
   ./scripts/test.sh
   ```
   Log file: `build/treeview/test/TreeViewTestLogs.log`


## Technologies Used

| **Technology**         | **Description**                                   |
|-------------------------|---------------------------------------------------|
| **Programming Language**| C++ 14                                             |
| **Build System**        | CMake                |
| **Testing Framework**   | Google Test                     |
| **Logging**       | spdlog                                           |
| **Version Control**     | Git for source code management                   |

## Future Enhancements
- **Visualization**: 
   - Show tree changes step-by-step for each insertion and deletion.
   - Add different drawing styles for nodes and connection arms.
   - Support non-monospaced font types for sharing over different apps.
- **BST Types**: Implement different BST balancing methods.
- **Sharing**:
   - Export to different content types (now only text file supported).
   - Allow reading tree content from file.