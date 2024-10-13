#pragma once
#include "gtest/gtest.h"
#include "DataStructures/Tree/TreeNode.h"

using namespace Ull;

class TestNode : public TreeNode<TestNode>
{
public:
	TestNode(const std::string& name) :
		TreeNode{ name }
	{
	}
};

TEST(NODE_TEST, EMPYTY_ON_INIT)
{
	TestNode testNode("TestNode");
	EXPECT_TRUE(testNode.GetChildren().empty());
}