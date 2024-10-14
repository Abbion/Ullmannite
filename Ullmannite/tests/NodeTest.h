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

TEST(NODE_TEST, NO_PARENT_ON_INIT)
{
	TestNode testNode("TestNode");
	EXPECT_EQ(testNode.GetParent().Get(), nullptr);
}

TEST(NODE_TEST, RETURNS_NAME)
{
	TestNode testNode("TestNode");
	EXPECT_EQ(testNode.GetName(), "TestNode");
}

TEST(NODE_TEST, ID_IS_HASHED)
{
	const auto nodeName = "TestNode";
	TestNode testNode(nodeName);

	const auto nodeHash = NodeId(std::hash<std::string>{}(nodeName));
	EXPECT_EQ(testNode.GetId(), nodeHash);
}

TEST(NODE_TEST, ID_IS_HASHED_PROPERLY)
{
	const auto nodeName1 = "TestNode";
	const auto nodeName2 = "BestNode";
	TestNode testNode(nodeName1);

	const auto nodeHash = NodeId(std::hash<std::string>{}(nodeName2));
	EXPECT_NE(testNode.GetId(), nodeHash);
}

TEST(NODE_TEST, GET_NULLOPT_NODE_BY_NAME)
{
	TestNode testNode("TestNode");
	EXPECT_EQ(testNode.GetNodeByName("Node"), std::nullopt);
}

TEST(NODE_TEST, GET_SELF_NODE_BY_NAME)
{
	TestNode testNode("TestNode");
	EXPECT_TRUE(testNode.GetNodeByName("TestNode").has_value());
}

TEST(NODE_TEST, GET_SELF_NODE_BY_NAME_CHECK_ID)
{
	const auto nodeName = "TestNode";

	TestNode testNode(nodeName);
	auto testNodeOpt = testNode.GetNodeByName(nodeName);
	auto testNodeRef = *testNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(nodeName));

	EXPECT_EQ(testNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_ONE_DEEP_NODE_BY_NAME)
{
	const auto oneDeepNodeName = "OneDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>(oneDeepNodeName);

	rootNode->AddChildNode(oneDeepNode);

	auto oneDeepNodeOpt = rootNode->GetNodeByName(oneDeepNodeName);

	EXPECT_TRUE(oneDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_ONE_DEEP_NODE_BY_NAME_AND_CHECK_ID)
{
	const auto oneDeepNodeName = "OneDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>(oneDeepNodeName);

	rootNode->AddChildNode(oneDeepNode);

	auto oneDeepNodeOpt = rootNode->GetNodeByName(oneDeepNodeName);
	auto oneDeepNodeRef = *oneDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(oneDeepNodeName));

	EXPECT_EQ(oneDeepNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_TWO_DEEP_NODE_BY_NAME)
{
	const auto twoDeepNodeName = "TwoDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>("OneDeepNode");
	auto twoDeepNode = std::make_shared<TestNode>(twoDeepNodeName);

	rootNode->AddChildNode(oneDeepNode);
	oneDeepNode->AddChildNode(twoDeepNode);

	auto twoDeepNodeOpt = rootNode->GetNodeByName(twoDeepNodeName);

	EXPECT_TRUE(twoDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_TWO_DEEP_NODE_BY_NAME_AND_CHECK_ID)
{
	const auto twoDeepNodeName = "TwoDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>("OneDeepNode");
	auto twoDeepNode = std::make_shared<TestNode>(twoDeepNodeName);

	rootNode->AddChildNode(oneDeepNode);
	oneDeepNode->AddChildNode(twoDeepNode);

	auto twoDeepNodeOpt = rootNode->GetNodeByName(twoDeepNodeName);
	auto twoDeepNodeRef = *twoDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(twoDeepNodeName));

	EXPECT_EQ(twoDeepNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_TWO_DEEP_NODE_BY_NAME_WITH_TWO_BRANCHES)
{
	const auto twoDeepNodeName = "TwoDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>("LeftOneDeepNode");
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");
	auto twoDeepNode = std::make_shared<TestNode>(twoDeepNodeName);

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);
	rightOneDeepNode->AddChildNode(twoDeepNode);

	auto twoDeepNodeOpt = rootNode->GetNodeByName(twoDeepNodeName);

	EXPECT_TRUE(twoDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_TWO_DEEP_NODE_BY_NAME_WITH_TWO_BRANCHES_AND_CHECK_ID)
{
	const auto twoDeepNodeName = "TwoDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>("LeftOneDeepNode");
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");
	auto twoDeepNode = std::make_shared<TestNode>(twoDeepNodeName);

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);
	rightOneDeepNode->AddChildNode(twoDeepNode);

	auto twoDeepNodeOpt = rootNode->GetNodeByName(twoDeepNodeName);
	auto twoDeepNodeRef = *twoDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(twoDeepNodeName));

	EXPECT_EQ(twoDeepNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_THREE_DEEP_NODE_BY_NAME)
{
	const auto threeDeepNodeName = "ThreeDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>("OneDeepNode");
	auto twoDeepNode = std::make_shared<TestNode>("TwoDeepNode");
	auto threeDeepNode = std::make_shared<TestNode>(threeDeepNodeName);

	rootNode->AddChildNode(oneDeepNode);
	oneDeepNode->AddChildNode(twoDeepNode);
	twoDeepNode->AddChildNode(threeDeepNode);

	auto threeDeepNodeOpt = rootNode->GetNodeByName(threeDeepNodeName);

	EXPECT_TRUE(threeDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_THREE_DEEP_NODE_BY_NAME_AND_CHECK_ID)
{
	const auto threeDeepNodeName = "ThreeDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>("OneDeepNode");
	auto twoDeepNode = std::make_shared<TestNode>("TwoDeepNode");
	auto threeDeepNode = std::make_shared<TestNode>(threeDeepNodeName);

	rootNode->AddChildNode(oneDeepNode);
	oneDeepNode->AddChildNode(twoDeepNode);
	twoDeepNode->AddChildNode(threeDeepNode);

	auto threeDeepNodeOpt = rootNode->GetNodeByName(threeDeepNodeName);
	auto threeDeepNodeRef = *threeDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(threeDeepNodeName));

	EXPECT_EQ(threeDeepNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_THIRD_DEEP_NODE_BY_NAME_WITH_THREE_BRANCHES)
{
	const auto leftThreeDeepNodeName = "LeftThreeDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");
	auto rightTwoDeepNode = std::make_shared<TestNode>("RightTwoDeepNode");
	auto RightThreeDeepNode = std::make_shared<TestNode>("RightThreeDeepNodeName");
	auto MiddleOneDeepNode = std::make_shared<TestNode>("MiddleOneDeepNide");
	auto leftOneDeepNode = std::make_shared<TestNode>("LeftOneDeepNode");
	auto leftTwoDeepNode = std::make_shared<TestNode>("LeftTwoDeepNode");
	auto leftThreeDeepNode = std::make_shared<TestNode>(leftThreeDeepNodeName);

	rootNode->AddChildNode(leftOneDeepNode);
	leftOneDeepNode->AddChildNode(leftTwoDeepNode);
	leftTwoDeepNode->AddChildNode(leftThreeDeepNode);

	rootNode->AddChildNode(MiddleOneDeepNode);

	rootNode->AddChildNode(rightOneDeepNode);
	rightOneDeepNode->AddChildNode(rightTwoDeepNode);
	rightTwoDeepNode->AddChildNode(RightThreeDeepNode);

	auto threeDeepNodeOpt = rootNode->GetNodeByName(leftThreeDeepNodeName);

	EXPECT_TRUE(threeDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_THIRD_DEEP_NODE_BY_NAME_WITH_THREE_BRANCHES_AND_CHECK_ID)
{
	const auto leftThreeDeepNodeName = "LeftThreeDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");
	auto rightTwoDeepNode = std::make_shared<TestNode>("RightTwoDeepNode");
	auto RightThreeDeepNode = std::make_shared<TestNode>("RightThreeDeepNodeName");
	auto MiddleOneDeepNode = std::make_shared<TestNode>("MiddleOneDeepNide");
	auto leftOneDeepNode = std::make_shared<TestNode>("LeftOneDeepNode");
	auto leftTwoDeepNode = std::make_shared<TestNode>("LeftTwoDeepNode");
	auto leftThreeDeepNode = std::make_shared<TestNode>(leftThreeDeepNodeName);

	rootNode->AddChildNode(leftOneDeepNode);
	leftOneDeepNode->AddChildNode(leftTwoDeepNode);
	leftTwoDeepNode->AddChildNode(leftThreeDeepNode);

	rootNode->AddChildNode(MiddleOneDeepNode);

	rootNode->AddChildNode(rightOneDeepNode);
	rightOneDeepNode->AddChildNode(rightTwoDeepNode);
	rightTwoDeepNode->AddChildNode(RightThreeDeepNode);

	auto threeDeepNodeOpt = rootNode->GetNodeByName(leftThreeDeepNodeName);
	auto threeDeepNodeRef = *threeDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(leftThreeDeepNodeName));

	EXPECT_EQ(threeDeepNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_NODE_FROM_SAME_LEVEL_NULLOPT)
{
	const auto leftOneDeepNodeName = "LeftOneDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>(leftOneDeepNodeName);
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);

	auto twoDeepNodeOpt = rightOneDeepNode->GetNodeByName(leftOneDeepNodeName);

	EXPECT_FALSE(twoDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_NODE_FROM_SAME_LEVEL_BY_PARENT)
{
	const auto leftOneDeepNodeName = "LeftOneDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>(leftOneDeepNodeName);
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);

	auto parentNode = leftOneDeepNode->GetParent();

	auto twoDeepNodeOpt = parentNode->GetNodeByName(leftOneDeepNodeName);

	EXPECT_TRUE(twoDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_NODE_FROM_SAME_LEVEL_BY_PARENT_AND_CHECK_ID)
{
	const auto leftOneDeepNodeName = "LeftOneDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>(leftOneDeepNodeName);
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);

	auto parentNode = leftOneDeepNode->GetParent();

	auto twoDeepNodeOpt = parentNode->GetNodeByName(leftOneDeepNodeName);
	auto twoDeepNodeRef = *twoDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(leftOneDeepNodeName));

	EXPECT_EQ(twoDeepNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_NODE_FROM_SAME_LEVEL_BY_ROOT)
{
	const auto leftOneDeepNodeName = "LeftOneDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>(leftOneDeepNodeName);
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);

	auto rootNodeRef = leftOneDeepNode->GetRoot();

	auto twoDeepNodeOpt = rootNodeRef->GetNodeByName(leftOneDeepNodeName);

	EXPECT_TRUE(twoDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_NODE_FROM_SAME_LEVEL_BY_ROOT_AND_CHECK_ID)
{
	const auto leftOneDeepNodeName = "LeftOneDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>(leftOneDeepNodeName);
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);

	auto rootNodeRef = leftOneDeepNode->GetRoot();

	auto leftTwoDeepNodeOpt = rootNodeRef->GetNodeByName(leftOneDeepNodeName);
	auto leftTwoDeepNodeRef = *leftTwoDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(leftOneDeepNodeName));

	EXPECT_EQ(leftTwoDeepNodeRef->GetId(), nodeHash);
}

TEST(NODE_TEST, GET_NODE_FROM_SAME_LEVEL_WITH_THREE_LEVELS_BY_ROOT)
{
	const auto rightTwoDeepNodeName = "RightTwoDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>("LeftOneDeepNode");
	auto leftTwoDeepNode = std::make_shared<TestNode>("LeftTwoDeepNode");
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");
	auto rightTwoDeepNode = std::make_shared<TestNode>(rightTwoDeepNodeName);

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);

	leftOneDeepNode->AddChildNode(leftTwoDeepNode);
	rightOneDeepNode->AddChildNode(rightTwoDeepNode);

	auto rootNodeRef = leftTwoDeepNode->GetRoot();

	auto rightTwoDeepNodeOpt = rootNodeRef->GetNodeByName(rightTwoDeepNodeName);

	EXPECT_TRUE(rightTwoDeepNodeOpt.has_value());
}

TEST(NODE_TEST, GET_NODE_FROM_SAME_LEVEL_WITH_THREE_LEVELS_BY_ROOT_AND_CHECK_ID)
{
	const auto rightTwoDeepNodeName = "RightTwoDeepNode";

	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto leftOneDeepNode = std::make_shared<TestNode>("LeftOneDeepNode");
	auto leftTwoDeepNode = std::make_shared<TestNode>("LeftTwoDeepNode");
	auto rightOneDeepNode = std::make_shared<TestNode>("RightOneDeepNode");
	auto rightTwoDeepNode = std::make_shared<TestNode>(rightTwoDeepNodeName);

	rootNode->AddChildNode(leftOneDeepNode);
	rootNode->AddChildNode(rightOneDeepNode);

	leftOneDeepNode->AddChildNode(leftTwoDeepNode);
	rightOneDeepNode->AddChildNode(rightTwoDeepNode);

	auto rootNodeRef = leftTwoDeepNode->GetRoot();

	auto rightTwoDeepNodeOpt = rootNodeRef->GetNodeByName(rightTwoDeepNodeName);
	auto rightTwoDeepNodeRef = *rightTwoDeepNodeOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(rightTwoDeepNodeName));

	EXPECT_EQ(rightTwoDeepNodeRef->GetId(), nodeHash);
}
 
TEST(NODE_TEST, GET_ROOT_FORM_SINGLE_NODE)
{
	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto rootNodeRef = rootNode->GetRoot();

	EXPECT_EQ(rootNodeRef.Get(), rootNode.get());
}

TEST(NODE_TEST, GET_ROOT_ON_FIRST_ABOVE)
{
	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>("OneDeepRootNode");

	rootNode->AddChildNode(oneDeepNode);
	
	auto rootNodeRef = oneDeepNode->GetRoot();

	EXPECT_EQ(rootNodeRef.Get(), rootNode.get());
}

TEST(NODE_TEST, GET_ROOT_ON_SECOND_ABOVE)
{
	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode = std::make_shared<TestNode>("OneDeepRootNode");
	auto twoDeepNode = std::make_shared<TestNode>("TwoDeepRootNode");

	rootNode->AddChildNode(oneDeepNode);
	oneDeepNode->AddChildNode(twoDeepNode);

	auto rootNodeRef = twoDeepNode->GetRoot();

	EXPECT_EQ(rootNodeRef.Get(), rootNode.get());
}

TEST(NODE_TEST, ATTACH_TWO_BRANCHES_GET_PARENT)
{
	auto rootNode1 = std::make_shared<TestNode>("RootNode1");
	auto oneDeepNode1 = std::make_shared<TestNode>("OneDeepRootNode1");

	rootNode1->AddChildNode(oneDeepNode1);

	auto rootNode2 = std::make_shared<TestNode>("RootNode2");
	auto leftOneDeepNode2 = std::make_shared<TestNode>("LeftOneDeepRootNode2");
	auto rightOneDeepNode2 = std::make_shared<TestNode>("RightOneDeepRootNode2");

	rootNode2->AddChildNode(leftOneDeepNode2);
	rootNode2->AddChildNode(rightOneDeepNode2);

	rootNode1->AddChildNode(rootNode2);

	auto root1NodeRef = leftOneDeepNode2->GetRoot();

	EXPECT_EQ(root1NodeRef.Get(), rootNode1.get());
}

TEST(NODE_TEST, ATTACH_TWO_BRANCHES_GET_BOTTOM_NODE_FROM_ROOT)
{
	auto rightOneDeepNode2Name = "RightOneDeepRootNode2";

	auto rootNode1 = std::make_shared<TestNode>("RootNode1");
	auto oneDeepNode1 = std::make_shared<TestNode>("OneDeepRootNode1");

	rootNode1->AddChildNode(oneDeepNode1);

	auto rootNode2 = std::make_shared<TestNode>("RootNode2");
	auto leftOneDeepNode2 = std::make_shared<TestNode>("LeftOneDeepRootNode2");
	auto rightOneDeepNode2 = std::make_shared<TestNode>(rightOneDeepNode2Name);

	rootNode2->AddChildNode(leftOneDeepNode2);
	rootNode2->AddChildNode(rightOneDeepNode2);

	rootNode1->AddChildNode(rootNode2);

	auto root1NodeRef = leftOneDeepNode2->GetRoot();
	auto rightOneDeepNode2Ref = rootNode1->GetNodeByName(rightOneDeepNode2Name);

	EXPECT_TRUE(rightOneDeepNode2Ref);
}

TEST(NODE_TEST, ATTACH_TWO_BRANCHES_GET_BOTTOM_NODE_FROM_ROOT_AND_CHECK_ID)
{
	auto rightOneDeepNode2Name = "RightOneDeepRootNode2";

	auto rootNode1 = std::make_shared<TestNode>("RootNode1");
	auto oneDeepNode1 = std::make_shared<TestNode>("OneDeepRootNode1");

	rootNode1->AddChildNode(oneDeepNode1);

	auto rootNode2 = std::make_shared<TestNode>("RootNode2");
	auto leftOneDeepNode2 = std::make_shared<TestNode>("LeftOneDeepRootNode2");
	auto rightOneDeepNode2 = std::make_shared<TestNode>(rightOneDeepNode2Name);

	rootNode2->AddChildNode(leftOneDeepNode2);
	rootNode2->AddChildNode(rightOneDeepNode2);

	rootNode1->AddChildNode(rootNode2);

	auto root1NodeRef = leftOneDeepNode2->GetRoot();
	auto rightOneDeepNode2RefOpt = rootNode1->GetNodeByName(rightOneDeepNode2Name);
	auto rightOneDeepNode2RefRef = *rightOneDeepNode2RefOpt;

	const auto nodeHash = NodeId(std::hash<std::string>{}(rightOneDeepNode2Name));

	EXPECT_EQ(rightOneDeepNode2RefRef->GetId(), nodeHash);
}

TEST(NODE_TEST, INSERT_NODE_WITH_EXISTING_ID_DEEP)
{
	auto rootNode = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode1 = std::make_shared<TestNode>("OneDeepRootNode");
	auto oneDeepNode2 = std::make_shared<TestNode>("OneDeepRootNode");

	rootNode->AddChildNode(oneDeepNode1);
	rootNode->AddChildNode(oneDeepNode2);

	EXPECT_EQ(rootNode->GetChildren().size(), 1);
}

TEST(NODE_TEST, INSERT_NODE_WITH_EXISTING_ID_ABOVE)
{
	auto rootNode1 = std::make_shared<TestNode>("RootNode");
	auto oneDeepNode1 = std::make_shared<TestNode>("OneDeepRootNode");
	auto rootNode2 = std::make_shared<TestNode>("RootNode");

	rootNode1->AddChildNode(oneDeepNode1);
	rootNode2->AddChildNode(rootNode1);

	EXPECT_EQ(rootNode2->GetChildren().size(), 0);
}