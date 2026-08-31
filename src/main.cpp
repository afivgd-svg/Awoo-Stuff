#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
#include <Geode/modify/CCLabelBMFont.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;

// very messy code ngl lmao. wait no i mean all this code lmao. atleast it's better than early 2.0 levels

class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) { // "Activated", "Selected", "Unselected"
	void activate() {
		std::string awooOnConfig = Mod::get()->getSettingValue<std::string>("awoo-on");
		if (awooOnConfig == "Activated") MyCCMenuItemSpriteExtra::awoo();

		CCMenuItemSpriteExtra::activate();
	}

	void selected() {
		std::string awooOnConfig = Mod::get()->getSettingValue<std::string>("awoo-on");
		if (awooOnConfig == "Selected") MyCCMenuItemSpriteExtra::awoo();

		CCMenuItemSpriteExtra::selected();
	}

	void unselected() {
		std::string awooOnConfig = Mod::get()->getSettingValue<std::string>("awoo-on");
		if (awooOnConfig == "Unselected") MyCCMenuItemSpriteExtra::awoo();

		CCMenuItemSpriteExtra::unselected();
	}

	void awoo() { // awoo? yes Naiko, awoo. wait this commment is copy past-
		bool isAwooEnabled = Mod::get()->getSettingValue<bool>("bool-enable-awoo");
		if (isAwooEnabled) {
			std::filesystem::path audioFilePath = Mod::get()->getSettingValue<std::filesystem::path>("custom-audio-path");
			if (audioFilePath.empty()) audioFilePath = Mod::get()->getResourcesDir() / "flea-awoo.mp3"; // WHY TF DOES IT DIVIDE BROW

			float audioVolume = Mod::get()->getSettingValue<float>("custom-button-sound-volume");

			if (std::filesystem::exists(audioFilePath)) FMODAudioEngine::sharedEngine()->playEffect(audioFilePath.string(), 1, 0, audioVolume);
		}
	}
};

// stolen from Geometwy Dash because yes :3 atleast Naiko happy lol AWOO!
// this should not exist. sscrappoed because its so stoopeed
/*class $modify(CCLabelBMFont) {
	static CCLabelBMFont* create(char const* str, char const* fntFile) {
		//fmt::format awooStr(str);
		//fmt::format endText = Mod::get()->getSettingValue<fmt::format>("custom-text-at-the-end-text");

		//bool doWeAwoo = Mod::get()->getSettingValue<bool>("add-awoo-to-end-text");
		//if (doWeAwoo) return CCLabelBMFont::create((awooStr + " " + endText).c_str(), fntFile);

		

		return CCLabelBMFont::create(str, fntFile);
	}
};


		"add-awoo-to-end-text": {
    		"type": "bool",
    		"name": "Toggle Custom End Text",
    		"description": "Add 'Awoo' at every text at the end. because why not? :3",
    		"default": false
		},
		"custom-text-at-the-end-text": {
			"type": "string",
			"name": "Custom Text",
			"enable-if": "add-awoo-to-end-text",
			"default": "awoo"
		},
		
- add text at the end of every text (execpt text that change)

*/

class $modify(PlayLayer) {
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}
		
    	auto winSize = CCDirector::sharedDirector()->getWinSize();

		bool isJumpscare = Mod::get()->getSettingValue<bool>("bool-enable-awoo-jumpscare");

		if (isJumpscare) {
			std::filesystem::path fleaImgPath = Mod::get()->getSettingValue<std::filesystem::path>("flea-jumpscare-img");
			if (fleaImgPath.empty()) {
				fleaImgPath = Mod::get()->getResourcesDir() / "flea-guy-idk.png"; // WHY TF DOES IT DIVIDE BROW
			}

			if (std::filesystem::exists(fleaImgPath)) {
				auto jumpscareSprite = CCSprite::create(fleaImgPath.string().c_str());
				jumpscareSprite->setPosition({ winSize.width / 2, winSize.height / 2 });
				jumpscareSprite->setScaleX(winSize.width / jumpscareSprite->getContentSize().width);
				jumpscareSprite->setScaleY(winSize.height / jumpscareSprite->getContentSize().height);
				jumpscareSprite->setOpacity(0);
				jumpscareSprite->setID("jumps"_spr); // DON'T ASK ME. don't...
				this->m_uiLayer->addChild(jumpscareSprite, 9999);
			}
		}
		
		return true;
	}
};

class $modify(MyPlayerObject, PlayerObject) {
	struct Fields {
		bool hasJumped = false; // The original name was "hasClickedThatJumpedYou" lmao
	};

	bool pushButton(PlayerButton button) {
		bool ret = PlayerObject::pushButton(button); // ya W Click Sounds lmao
		if (button != PlayerButton::Jump) return ret;

		PlayLayer* playLayer = PlayLayer::get();
		if (!playLayer) return ret;

		bool doWeDoJumpscare = Mod::get()->getSettingValue<bool>("bool-enable-awoo-jumpscare");
		bool doWeAwoo = Mod::get()->getSettingValue<bool>("bool-enable-awoo-on-jump");

		auto sprite = (CCSprite*)playLayer->m_uiLayer->getChildByID("jumps"_spr);
		if (!sprite) doWeDoJumpscare = false;

		if (doWeDoJumpscare || doWeAwoo) {
			if ((m_isShip || m_isBird || m_isDart || m_isSwing)) { // uh.. basically isCube, isRobot, isBall, or isSpider lmao. wait no. NO WAY I REMOVED THE EXCLAMATION MARQ
				if (doWeDoJumpscare) {
					MyPlayerObject::duar(sprite);
				}

				if (doWeAwoo) {
					MyPlayerObject::awoo();
				}
			}
		}

		//if (this->m_isOnGround && !(m_isShip && m_isBird && m_isBall && m_isDart && m_isRobot && m_isSpider && m_isSwing)) { // idk brow. maybe this is the only way

		//}

		return ret;
	}

	void ringJump(RingObject* object, bool skipCheck) {
		PlayerObject::ringJump(object, skipCheck);

		if (m_holdingButtons[1] && !m_fields->hasJumped) {
			PlayLayer* playLayer = PlayLayer::get();
			if (!playLayer) { PlayerObject::ringJump(object, skipCheck); return; }

			bool doWeDoJumpscare = Mod::get()->getSettingValue<bool>("bool-enable-awoo-jumpscare");
			bool doWeAwoo = Mod::get()->getSettingValue<bool>("bool-enable-awoo-on-jump");

			auto sprite = (CCSprite*)playLayer->m_uiLayer->getChildByID("jumps"_spr);
			if (!sprite) doWeDoJumpscare = false;

			if (doWeDoJumpscare || doWeAwoo) {
				if (!(m_isShip || m_isBird || m_isDart || m_isSwing)) { // i like copy and paste :3
					if (doWeDoJumpscare) {
						MyPlayerObject::duar(sprite);
					}

					if (doWeAwoo) {
						MyPlayerObject::awoo();
					}
				}
			}
			
			m_fields->hasJumped = true;
		}
	}

	void bumpPlayer(float bumpMod, int objectType, bool noEffects, GameObject* object) {
		PlayerObject::bumpPlayer(bumpMod, objectType, noEffects, object);

		PlayLayer* playLayer = PlayLayer::get();
		if (!playLayer) return;

		bool doWeDoJumpscare = Mod::get()->getSettingValue<bool>("bool-enable-awoo-jumpscare");
		bool doWeAwoo = Mod::get()->getSettingValue<bool>("bool-enable-awoo-on-jump");

		auto sprite = (CCSprite*)playLayer->m_uiLayer->getChildByID("jumps"_spr);
		if (!sprite) doWeDoJumpscare = false;

		if (doWeDoJumpscare || doWeAwoo) {
			if (doWeDoJumpscare) {
				MyPlayerObject::duar(sprite);
			}

			if (doWeAwoo) {
				MyPlayerObject::awoo();
			}
		}
	}

	void updateJump(float dt) {
		if (m_holdingButtons[1]) {
			if (m_isOnGround) {
				if (!(m_isShip || m_isBall || m_isBird || m_isDart || m_isRobot || m_isSwing) || ((m_isBall || m_isRobot || m_isSpider) && !m_fields->hasJumped)) {
					PlayLayer* playLayer = PlayLayer::get();
					if (!playLayer) { PlayerObject::updateJump(dt); return; }

					bool doWeDoJumpscare = Mod::get()->getSettingValue<bool>("bool-enable-awoo-jumpscare");
					bool doWeAwoo = Mod::get()->getSettingValue<bool>("bool-enable-awoo-on-jump");

					auto sprite = (CCSprite*)playLayer->m_uiLayer->getChildByID("jumps"_spr);
					if (!sprite) doWeDoJumpscare = false;


					if (doWeDoJumpscare) {
						MyPlayerObject::duar(sprite);
					}

					if (doWeAwoo) {
						MyPlayerObject::awoo();
					}
					
					m_fields->hasJumped = true;
				}
			}
		}
		else {
			m_fields->hasJumped = false;
		}

		PlayerObject::updateJump(dt);
	}

	void duar(CCSprite* fg) { // kaboom? yes Riko, kaboom
		fg->stopActionByTag(872355); // can a tag be this long? btw, its reference from. the bite of rah, whats funnier than 22, andd king sammelottest
		fg->setOpacity(255);
		auto action = CCFadeTo::create(0.5f, 0);
		action->setTag(872355);
		fg->runAction(action);
	}

	void awoo() { // awoo? yes Naiko, awoo
		std::filesystem::path audioFilePath = Mod::get()->getSettingValue<std::filesystem::path>("custom-audio-path");
		if (audioFilePath.empty()) audioFilePath = Mod::get()->getResourcesDir() / "flea-awoo.mp3"; // WHY TF DOES IT DIVIDE BROW

		float audioVolume = Mod::get()->getSettingValue<float>("custom-button-sound-volume");

		if (std::filesystem::exists(audioFilePath)) FMODAudioEngine::sharedEngine()->playEffect(audioFilePath.string(), 1, 0, audioVolume);
	}
};
