#include "cocos2d.h"
USING_NS_CC;

class GamePlayingScene :public cocos2d::Scene{
public:
	//CREATE_WITH_PHYSICS_FUNC(GamePlayingScene);
	void initial();
	static GamePlayingScene* createScene();

	//void initial();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void updateTimeForProgressBar(float dt);
	bool onContactBegan(PhysicsContact& contact);

	void ClickGameEnded(Ref* sender);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event*event);
private:
	Sprite* backgroundSprite;
	float totalTime;
};