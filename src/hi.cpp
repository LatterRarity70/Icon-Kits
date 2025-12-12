#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

using namespace geode::prelude;

/*
* score:
IconType m_iconType;
int m_color1;
int m_color2;
int m_color3;
int m_special;
int m_playerCube;
int m_playerShip;
int m_playerBall;
int m_playerUfo;
int m_playerWave;
int m_playerRobot;
int m_playerSpider;
int m_playerSwing;
int m_playerJetpack;
int m_playerStreak;
bool m_glowEnabled;
int m_playerExplosion;
* gm:
geode::SeedValueRSV m_playerFrame;
geode::SeedValueRSV m_playerShip;
geode::SeedValueRSV m_playerBall;
geode::SeedValueRSV m_playerBird;
geode::SeedValueRSV m_playerDart;
geode::SeedValueRSV m_playerRobot;
geode::SeedValueRSV m_playerSpider;
geode::SeedValueRSV m_playerSwing;
geode::SeedValueRSV m_playerColor;
geode::SeedValueRSV m_playerColor2;
geode::SeedValueRSV m_playerGlowColor;
geode::SeedValueRSV m_playerStreak;
geode::SeedValueRSV m_playerShipFire;
geode::SeedValueRSV m_playerDeathEffect;
geode::SeedValueRSV m_playerJetpack;
bool m_playerGlow;
IconType m_playerIconType;
*/
inline static auto supportedModIDs = { ""
    ,"hiimjustin000.more_icons"
    ,"weebify.separate_dual_icons"
    ,"naxrin.chroma_icons" //this thing have big as fuck defaults
};

class IconKitsPopup : public geode::Popup<> {
public:
    static void applyIconKit(GJUserScore* sc) {
        GameManager* gm = GameManager::get();
        gm->m_playerFrame = sc->m_playerCube;
		gm->m_playerShip = sc->m_playerShip;
		gm->m_playerBall = sc->m_playerBall;
		gm->m_playerBird = sc->m_playerUfo;
        gm->m_playerDart = sc->m_playerWave;
		gm->m_playerRobot = sc->m_playerRobot;
		gm->m_playerSpider = sc->m_playerSpider;
		gm->m_playerSwing = sc->m_playerSwing;
		gm->m_playerColor = sc->m_color1;
		gm->m_playerColor2 = sc->m_color2;
		gm->m_playerGlowColor = sc->m_color3 ? sc->m_color3 : sc->m_color2;
		gm->m_playerStreak = sc->m_playerStreak;
		gm->m_playerJetpack = sc->m_playerJetpack;
		gm->m_playerGlow = sc->m_glowEnabled;
		gm->m_playerIconType = sc->m_iconType;
		gm->m_playerDeathEffect = sc->m_playerExplosion;
        auto val = matjson::parse(sc->getID()).unwrapOr(0);
        for (auto& id : supportedModIDs) if (auto mod = Loader::get(
        )->getLoadedMod(id)) {
            for (auto& entry : val[id]) mod->setSavedValue(
                entry.getKey().value_or(""), entry
            );
            mod->saveData().isOk();
        }
        if (Ref a = CCScene::get()->getChildByType<GJGarageLayer>(0)) {
            a->getParent()->addChild(GJGarageLayer::node());
            a->removeFromParent();
        }
        Notification::create("Your Icon Kit Updated")->show();
    }
    ScrollLayer* m_scroll = nullptr;
    void setupList() {
        if (m_scroll) if (auto container = m_scroll->m_contentLayer) {
            container->removeAllChildren();
            auto list = getMod()->getSavedValue("list", matjson::Value::array());
            for (auto& entry : list) {
                Ref score = GJUserScore::create();
                if (score) {
                    score->setID(entry.dump());
                    score->m_playerCube = entry["cube"].as<int>().unwrapOrDefault();
                    score->m_playerShip = entry["ship"].as<int>().unwrapOrDefault();
                    score->m_playerBall = entry["ball"].as<int>().unwrapOrDefault();
                    score->m_playerUfo = entry["bird"].as<int>().unwrapOrDefault();
                    score->m_playerWave = entry["dart"].as<int>().unwrapOrDefault();
                    score->m_playerRobot = entry["robot"].as<int>().unwrapOrDefault();
                    score->m_playerSpider = entry["spider"].as<int>().unwrapOrDefault();
                    score->m_playerSwing = entry["swing"].as<int>().unwrapOrDefault();
                    score->m_playerJetpack = entry["jetpack"].as<int>().unwrapOrDefault();
                    score->m_color1 = entry["color"].as<int>().unwrapOrDefault();
                    score->m_color2 = entry["color2"].as<int>().unwrapOrDefault();
                    score->m_color3 = entry["glow"].as<int>().unwrapOrDefault();
                    score->m_playerStreak = entry["streak"].as<int>().unwrapOrDefault();
                    score->m_playerExplosion = entry["explode"].as<int>().unwrapOrDefault();
                    score->m_glowEnabled = entry["glow"].as<bool>().unwrapOrDefault();
                    score->m_iconType = (IconType)entry["icontype"].as<int>().unwrapOrDefault();
                };
                /* xd
                Ref pp = ProfilePage::create(-hash(entry.dump()), 1);
                pp->loadPageFromUserInfo(score);

                Ref menu = pp->querySelector("player-menu");
                menu->setUserObject("ProfilePage", pp);
                menu->removeFromParentAndCleanup(0);*/
                Ref menu = CCMenu::create();
                if (menu) {
					menu->setContentSize(ccp(340, 42));
                    auto icons = std::vector<int>{
                        score->m_playerCube,
                        score->m_playerShip,
                        score->m_playerBall,
                        score->m_playerUfo,
                        score->m_playerWave,
                        score->m_playerRobot,
                        score->m_playerSpider,
                        score->m_playerSwing,
                        score->m_playerJetpack
                    };
                    for (auto id : icons) { //last: Jetpack = 8
                        auto type = (IconType)menu->getChildrenCount();
                        Ref ae = SimplePlayer::create(id);
                        ae->updatePlayerFrame(id, type);
                        ae->setColors(
                            GameManager::get()->colorForIdx(score->m_color1),
							GameManager::get()->colorForIdx(score->m_color2)
                        );
                        ae->setGlowOutline(GameManager::get()->colorForIdx(
                            score->m_color3 ? score->m_color3 : score->m_color2
                        ));
                        ae->updateColors();
                        ae->setContentSize(ae->getChildByIndex(0)->getContentSize());
                        queueInMainThread([ae]() { if (ae) ae->setAnchorPoint(CCPointZero); });
                        menu->addChild(ae);
                        if (MoreIcons::loaded()) {
                            auto val = matjson::parse(
                                entry["hiimjustin000.more_icons"]
                                .as<std::string>().unwrapOrDefault()
                            ).unwrapOrDefault();
                            MoreIcons::updateSimplePlayer(
                                ae, val[MoreIcons::saveKey(type, 0
                                )].as<std::string>().unwrapOr(""), type
                            );
                        }
                    }
                    menu->addChild(GJItemIcon::createBrowserItem(
                        UnlockType::Streak, score->m_playerStreak
                    ));
                    menu->addChild(GJItemIcon::createBrowserItem(
                        UnlockType::Death, score->m_playerExplosion
                    ));
                    if (auto btn = CCMenuItemExt::createSpriteExtraWithFrameName(
                        "geode.loader/save.png", 0.750f, [score, entry](void*) {
                            std::stringstream ss;
                            for (auto id : supportedModIDs) if (auto mod = Loader::get(
                            )->getLoadedMod(id)) {
								ss << "## [" << mod->getName() << "](mod:" << id << ")\n";
                                ss << "```json\n";
                                ss << matjson::parse(
                                    entry[id].as<std::string>().unwrapOrDefault()
                                ).unwrapOrDefault().dump();
								ss << "\n```\n";
                            }
                            MDPopup::create(
                                "Other Saved Data (Mods Support)",
                                ss.str(), "Ok"
                            )->show();
                        }
                    )) {
                        menu->addChild(btn);
                    }
                    menu->addChild(CCLayerColor::create({ 0,0,0,0 }, 6, 5), -2);
                    menu->addChild(CCLayerColor::create({ 0,0,0,0 }, 12, 5), 2);
                    menu->setLayout(RowLayout::create()
                        ->setGap(12.000f)
                        ->setCrossAxisOverflow(0)
                    );
                }

                menu->setContentWidth(64 + menu->getContentWidth());
                if (auto bg = CCScale9Sprite::create("square02_001.png")) {
                    bg->setPosition(menu->getContentSize() / 2);
                    bg->setContentSize(menu->getContentSize());
					bg->setColor(ccBLACK);
					bg->setOpacity(46);
                    menu->addChild(bg, -3);
                }

                if (auto line = CCSprite::createWithSpriteFrameName(
                    "edit_vLine_001.png"
                )) {
                    line->setPosition(ccp(337.000f, 20.000f));
                    line->setScale(0.425f);
                    menu->addChild(line, 1);
                }

                menu->setContentHeight(4 + menu->getContentHeight());
                container->addChild(menu, 1);
                handleTouchPriority(menu);

                menu->addChild(score);
                if (auto btn = CCMenuItemExt::createSpriteExtraWithFrameName(
                    "GJ_selectSongBtn_001.png", 0.750f, [score](void*) {
                        applyIconKit(score);
                    }
                )) {
                    btn->setPosition(358.000f, 21.000f);
                    btn->updateSprite();
                    menu->addChild(btn);
                }

                menu->setTag(container->getChildrenCount());
                if (auto btn = CCMenuItemExt::createSpriteExtraWithFrameName(
                    "GJ_orderUpBtn_001.png", 0.375f, [menu, _this = Ref(this)](void*) {
                        auto list = getMod()->getSavedValue(
                            "list", matjson::Value::array()
                        );
                        auto newl = matjson::Value::array();
                        auto idx = menu->getTag();
                        auto tar = matjson::Value();
                        auto others = std::vector<matjson::Value>();
                        for (auto& entry : list) {
                            if (--idx == 0) tar = entry;
                            else others.push_back(entry);
                        }
						for (auto& entry : others) newl.push(entry);
						newl.push(tar);
                        getMod()->setSavedValue<matjson::Value>("list", newl);
                        getMod()->saveData().isOk();
                        _this->setupList();
                    }
                )) {
                    btn->setPosition(386.000f, 29.000f);
                    btn->updateSprite();
                    menu->addChild(btn);
                }
                if (auto btn = CCMenuItemExt::createSpriteExtraWithFrameName(
                    "GJ_resetBtn_001.png", 0.600f, [menu, _this = Ref(this)](void*) {
                        auto list = getMod()->getSavedValue(
                            "list", matjson::Value::array()
                        );
                        auto newl = matjson::Value::array();
                        auto idx = menu->getTag();
                        for (auto& entry : list) {
                            if (--idx == 0) continue;
							newl.push(entry);
                        }
                        getMod()->setSavedValue<matjson::Value>("list", newl);
                        getMod()->saveData().isOk();
                        _this->setupList();
                    }
                )) {
                    btn->setPosition(386.000f, 10.000f);
                    btn->updateSprite();
                    menu->addChild(btn);
                }
            }
            container->setLayout(RowLayout::create()
                ->setGap(0.000f)
                ->setGrowCrossAxis(true)
                ->setCrossAxisReverse(true)
                ->setAxisAlignment(AxisAlignment::End)
            );
            auto spacer = CCNode::create();
            spacer->setContentWidth(m_scroll->getContentWidth());
			spacer->setContentHeight(
                m_scroll->getContentHeight() - container->getContentHeight()
            );
			container->addChild(spacer);
            container->updateLayout();
        }
    }
    ProfilePage* m_profile = nullptr;
    void onSaveCurrent() {
        if (!m_profile) m_profile = CCScene::get()->getChildByType<ProfilePage>(0);
        GJUserScore* sc = (m_profile ? m_profile->m_score : nullptr);
        GameManager* gm = GameManager::get();
        auto entry = matjson::Value::object();
        {
            entry["cube"] = sc ? sc->m_playerCube : gm->m_playerFrame;
            entry["ship"] = sc ? sc->m_playerShip : gm->m_playerShip;
            entry["ball"] = sc ? sc->m_playerBall : gm->m_playerBall;
            entry["bird"] = sc ? sc->m_playerUfo : gm->m_playerBird;
            entry["dart"] = sc ? sc->m_playerWave : gm->m_playerDart;
            entry["robot"] = sc ? sc->m_playerRobot : gm->m_playerRobot;
            entry["spider"] = sc ? sc->m_playerSpider : gm->m_playerSpider;
            entry["swing"] = sc ? sc->m_playerSwing : gm->m_playerSwing;
            entry["color"] = sc ? sc->m_color1 : gm->m_playerColor;
            entry["color2"] = sc ? sc->m_color2 : gm->m_playerColor2;
            entry["glow"] = sc ? sc->m_color3 ? sc->m_color3 : sc->m_color2 : gm->m_playerGlowColor;
            entry["streak"] = sc ? sc->m_playerStreak : gm->m_playerStreak;
            entry["explode"] = sc ? sc->m_playerExplosion : gm->m_playerDeathEffect;
            entry["jetpack"] = sc ? sc->m_playerJetpack : gm->m_playerJetpack;
            entry["glow"] = sc ? sc->m_glowEnabled : gm->m_playerGlow;
            entry["icontype"] = sc ? (int)sc->m_iconType : (int)gm->m_playerIconType;
            for (auto id : supportedModIDs) if (auto mod = Loader::get(
            )->getLoadedMod(id)) {
                mod->saveData().isOk();
                auto str = mod->getSaveContainer().dump(matjson::NO_INDENTATION);
                entry[id] = str;
            }
        };
        auto list = getMod()->getSavedValue("list", matjson::Value::array());
        list.push(entry);
        getMod()->setSavedValue<matjson::Value>("list", list);
        getMod()->saveData().isOk();
        setupList();
    }
    bool setup() override {
        this->setTitle("Icon Kits");

        m_scroll = ScrollLayer::create({342.000, 198.000});
        m_scroll->ignoreAnchorPointForPosition(false);
        m_mainLayer->addChildAtPosition(m_scroll, Anchor::Center);
        setupList();

        if (m_buttonMenu) {
            if (auto item = CCMenuItemExt::createSpriteExtra(
                ButtonSprite::create("Save Current"), [this](void*) {
                    onSaveCurrent();
                }
            )) {
                m_buttonMenu->addChildAtPosition(item, Anchor::Bottom);
                item->getNormalImage()->setScale(0.75f);
                item->updateSprite();
            }
        }

        return true;
    }
    static IconKitsPopup* create() {
        auto ret = new IconKitsPopup();
        if (ret->initAnchored(380.000f, 272.000f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

#include "Geode/modify/ProfilePage.hpp"//left-button-menu
class $modify(ProfilePageExt, ProfilePage) {
    void onUpdate(cocos2d::CCObject * sender) {
        if (m_accountID < -1) return;
        ProfilePage::onUpdate(sender);
    };
};

#include "Geode/modify/GJGarageLayer.hpp"//left-button-menu
class $modify(GJGarageLayerExt, GJGarageLayer) {
    void customSetup() {
        if (auto menu = querySelector("shards-menu")) {
            auto item = CCMenuItemExt::createSpriteExtraWithFrameName(
                "gj_folderBtn_001.png", 0.925f, [_this = Ref(this)](void*) {
                    if (!_this) return;
                    if (auto a = IconKitsPopup::create()) {
                        a->m_scene = _this;
                        a->show();
                    }
                }
            );
			menu->addChild(item);
            menu->updateLayout();
        }
    };
    virtual bool init() {
		if (!GJGarageLayer::init()) return false;
		customSetup();
		return true;
    }
};