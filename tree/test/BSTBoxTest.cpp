#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <sstream>

#include "../source/AVL.h"
#include "../source/BSTBox.h"

using spdlog::debug;
using std::stringstream;

TEST(BSTBoxTest, Present) {
    AVLNode* root = createAVLNode(1);
    insertAVLNode(root, 2);
    insertAVLNode(root, 3);

    BSTBox* bstBox = createBSTBox(root);
    stringstream os;

    presentBSTBox(os, bstBox);

    string expectedOutput = 
R"(          ___          
         |   |         
      ___| 2 |___      
     |   |___|   |     
     |           |     
    _|_         _|_    
   |   |       |   |   
   | 1 |       | 3 |   
   |___|       |___|   
                       
)";

    EXPECT_EQ(os.str(), expectedOutput);

    deleteBSTBox(bstBox);
    deleteAVLNode(root);
}