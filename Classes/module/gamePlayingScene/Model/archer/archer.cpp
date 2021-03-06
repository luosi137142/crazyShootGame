#include"archer.h"
#include"../../../../public/Constant/Constant.h"
#include"../../../../public/ParameterManager/ParameterManager.h"

archer* archer::archer_ = NULL;

archer::archer() {}

archer::~archer() {
	if (archer_ != NULL) {
		archer_ = NULL;
	}
}

archer* archer::getInstance() {
	if (archer_ == NULL) {
		archer_ = archer::create();
	}
	return archer_;
}

bool archer::init(){
	if (!Sprite::init()) { return false; }

	Size visibleSize = ParameterManager::getVisibleSize();
	float times = 1.5;//图形放大倍数

	body = Sprite::create(Constant::getArcherbodyPath());
	body->setPosition(0,0);
	body->setScale(times);
	this->addChild(body, 1);
	
	//auto bodyBody = PhysicsBody::createBox(body->getContentSize());
	//bodyBody->getFirstShape()->setTag(Constant::getArcherTag());
	//body->setPhysicsBody(bodyBody);
	//body->getPhysicsBody()->setContactTestBitmask(0x0000000F);
	//body->getPhysicsBody()->setCollisionBitmask(0x0000000F);
	

	hand = Sprite::create(Constant::getArcherhandPath());
	hand->setPosition(body->getPositionX() - (4 * times), body->getPositionY() - (3 * times));
	hand->setScale(times);
	this->addChild(hand, 3);
	
	//auto handBody = PhysicsBody::createBox(hand->getContentSize());
	//handBody->getFirstShape()->setTag(Constant::getArcherhandTag());
	//hand->setPhysicsBody(handBody);
	//hand->getPhysicsBody()->setContactTestBitmask(0x0000000F);
	//hand->getPhysicsBody()->setCollisionBitmask(0x0000000F);
	
	head = Sprite::create(Constant::getArcherheadPath());
	head->setPosition(body->getPositionX() - (1 * times), body->getPositionY() + (2 * times));
	head->setScale(times);
	this->addChild(head, 2);
	
	//auto headBody = PhysicsBody::createBox(head->getContentSize());
	//headBody->getFirstShape()->setTag(Constant::getArcherheadTag());
	//head->setPhysicsBody(headBody);
	//head->getPhysicsBody()->setContactTestBitmask(0x0000000F);
	//head->getPhysicsBody()->setCollisionBitmask(0x0000000F);
   
	//血条
	hpBar_back = Sprite::create(Constant::getBloodBackPath());
	hpBar_back->setPosition(Vec2(body->getContentSize().width / 2, body->getContentSize().height + 30));
	body->addChild(hpBar_back);

	hpBar = ProgressTimer::create(Sprite::create(Constant::getBloodForePath()));
	hpBar->setType(ProgressTimer::Type::BAR); // 设置进度条样式（条形或环形）
	hpBar->setMidpoint(Vec2(0, 0.5f));        // 设置进度条的起始点，（0，y）表示最左边，（1，y）表示最右边，（x，1）表示最上面，（x，0）表示最下面。
	hpBar->setBarChangeRate(Vec2(1, 0));      // 设置进度条变化方向，（1,0）表示横方向，（0,1）表示纵方向。
	hpBar->setPercentage(100);                // 设置当前进度条的进度
	hpBar->setPosition(Vec2(hpBar_back->getContentSize().width / 2, hpBar_back->getContentSize().height / 2));
	hpBar_back->addChild(hpBar);
	hpBar_back->setVisible(true);   // 设置整个血条可见，我们将在Player 遭受攻击的时候再显示血条。
	
	//add touch listener
	EventListenerTouchOneByOne* screenListener = EventListenerTouchOneByOne::create();
	screenListener->setSwallowTouches(false);// true不向下触摸，简单点来说，比如有两个sprite ,A和 B，A在上B在下（位置重叠）
	screenListener->onTouchBegan = CC_CALLBACK_2(archer::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(screenListener, this);
	/**
	body->getPhysicsBody()->setContactTestBitmask(0x0000000F);
	//archerSprite->getPhysicsBody()->setCollisionBitmask(0x0000000F);
	body->getPhysicsBody()->setCategoryBitmask(0x0000000F);
	body->getPhysicsBody()->getFirstShape()->setTag(Constant::getArcherTag());
	**/

	//整体
	
	auto bodyBody = PhysicsBody::createBox(Size(body->getContentSize().width * 0.8,body->getContentSize().height * 1.2));
	bodyBody->getFirstShape()->setTag(Constant::getArcherTag());
	this->setPhysicsBody(bodyBody);

	this->getPhysicsBody()->setContactTestBitmask(1);
	this->getPhysicsBody()->setCategoryBitmask(2); //种类

	//arrowSprite->getPhysicsBody()->setContactTestBitmask(0x0000F0F0);
	//arrowSprite->getPhysicsBody()->setCategoryBitmask(0x0000F0F0);
	
	return true;
}

bool archer::onTouchBegan(Touch *touch, Event *unused_event) {
	auto archerCenter = this->getPosition();
	auto touchPoint = touch->getLocation();
	if (touchPoint.x >= archerCenter.x) {
		body->setFlippedX(false);//不翻转
		head->setFlippedX(false);
		hand->setFlippedX(false);
		double angle;
		if (fabs(touchPoint.x - archerCenter.x) < 1e-6) {
			angle = 135;
		}
		else {
			//计算点击处与archer的连线与九点钟方向的顺时针夹角。
			angle = -atan((touchPoint.y - archerCenter.y) / (touchPoint.x - archerCenter.x)) / 3.141592654 * 180;
			angle += 180;
			if (angle < 135) angle = 135;//限制角的范围，不然如果要头往正上方看的话，图会很难看
			else if (angle > 225) angle = 225;
		}
		head->setRotation(angle - 180);
		hand->setRotation(angle + 90);
	}
	else {
		double angle = -atan((touchPoint.y - archerCenter.y) / (touchPoint.x - archerCenter.x)) / 3.141592654 * 180;
		body->setFlippedX(true);
		head->setFlippedX(true);
		hand->setFlippedX(true);
		if (angle > 45) angle = 45;
		else if (angle < -45) angle = -45;
		head->setRotation(angle);
		hand->setRotation(angle + 90);
	}
	return true;
}