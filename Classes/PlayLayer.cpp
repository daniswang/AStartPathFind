#include "PlayLayer.h"
#include "Node.h"

#define MAX_WIDETH  (7);
#define MAX_HEIGTH (9);
#define SUSHI_GAP (0.5)

PlayLayer::PlayLayer():m_width(0),m_height(0),m_IsTouchAble(true),m_State(0)
{
}

PlayLayer::~PlayLayer()
{
	if(m_matrix != NULL)
		free(m_matrix);
}

CCScene* PlayLayer::CreatScene()
{
	auto scene = CCScene::create();
	auto playLayer = create();
	scene->addChild(playLayer);
	return scene;
}

bool PlayLayer::init()
{
	if(!CCLayer::init())
		return false;
	
	//background
	CCSize WinSize =CCDirector::sharedDirector()->getWinSize();
	auto background = CCSprite::create("bg.png");
	background->setAnchorPoint(CCPoint(0, 1));
	background->setPosition(CCPoint(0, WinSize.height));
	addChild(background);

	//init spriteSheet;
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sushi.plist");
    //spriteSheet = SpriteBatchNode::create("sushi.pvr.ccz");
    //addChild(spriteSheet);

	m_width = MAX_WIDETH;
	m_height = MAX_HEIGTH;

	m_matrixLeftBottomX = (WinSize.width - Node::GetContentWidth() * m_width - (m_width - 1) * SUSHI_GAP) / 2;
    m_matrixLeftBottomY = (WinSize.height - Node::GetContentWidth() * m_height - (m_height - 1) * SUSHI_GAP) / 2;
	
	int arraySize = sizeof(Node*) * m_width * m_height;
	m_matrix = (Node**)malloc(arraySize);
	memset((void*)m_matrix, 0, arraySize);

	InitMatrix();

	//register touchevent
	registerWithTouchDispatcher();

	return true;
}

void PlayLayer::InitMatrix()
{
	for(int i = 0;i < m_height;i ++)
	{
		for(int l = 0;l < m_width;l ++)
		{
			Node* node = Node::Create(i, l);
			CCPoint point = PositionOfItem(i, l);
			node->setPosition(point);
			addChild(node);
			*(m_matrix + i * m_width + l) = node;
		}
	}
}

void PlayLayer::update(float dt)
{
	
}

/************** touch event************************/
void PlayLayer::registerWithTouchDispatcher(void)
{//register touch event
	CCLayer::registerWithTouchDispatcher();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
 
bool PlayLayer::ccTouchBegan(CCTouch *touch, CCEvent *unused)
{
	if(m_IsTouchAble)
	{
		auto location = touch->getLocation();
		Node* touchedNode = NodeOfPoint(&location);
		if(!touchedNode || !touchedNode->IsNodeInfoNormal())
			return false;

		if(touchedNode->getInfo() == NODE_NORMAL)
		{
			touchedNode->setInfo(NODE_SOURCE);
		}
		CCSprite* addNode = CCSprite::create("source.png");
		CCPoint addlocation = PositionOfItem(touchedNode->getRow(), touchedNode->getCol());
		addNode->setPosition(CCPoint(touchedNode->getContentSize().width/2, touchedNode->getContentSize().height/2));
		touchedNode->addChild(addNode);
	}
	return m_IsTouchAble;
}

void PlayLayer::ccTouchMoved(CCTouch *touch, CCEvent *unused)
{
	if(!m_IsTouchAble)
		return;
	//m_IsTouchAble = false;
}

void PlayLayer::ccTouchEnded(CCTouch *touch, CCEvent *unused)
{
	printf("end\n");
	if(m_IsTouchAble)
	{
		//m_IsTouchAble = false;
		
	}
	return;
}

void PlayLayer::ccTouchCancelled(CCTouch *touch, CCEvent *unused)
{
	return;
}
// touch end

CCPoint PlayLayer::PositionOfItem(int row, int col)
{
	 float x = m_matrixLeftBottomX + (Node::GetContentWidth() + SUSHI_GAP) * col + Node::GetContentWidth() / 2;
    float y = m_matrixLeftBottomY + (Node::GetContentWidth() + SUSHI_GAP) * row + Node::GetContentWidth() / 2;
    return CCPoint(x, y);
}

Node* PlayLayer::NodeOfPoint(CCPoint* point)
{
	Node* node = NULL;

	CCRect rect = CCRect(0, 0, 0, 0);
	for(int i = 0;i < m_height;i ++)
	{
		for(int l = 0;l < m_width;l ++)
		{
			Node* node = NULL;
			node = *(m_matrix+ i * m_width + l);
			if(node)
			{
				rect.origin.x = node->getPositionX() - (node->getContentSize().width / 2);
				rect.origin.y = node->getPositionY() - (node->getContentSize().height / 2);
				rect.size = node->getContentSize();
				if(rect.containsPoint(*point))
					return node;
			}
		}
	}

	return NULL;
}