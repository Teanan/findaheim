///
/// anax
/// An open source C++ entity system.
///
/// Copyright (C) 2013-2014 Miguel Martin (miguel@miguel-martin.com)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///

#include "Game.hpp"

#include <iostream>
#include <random>

#include <Components/TransformComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/AnimationComponent.hpp>
#include <Components/VelocityComponent.hpp>
#include <Components/PhysicsComponent.h>
#include <Components/LightComponent.h>

#include "TexturesManager.h"

const sf::Color CLEAR_COLOR = sf::Color::White;
const unsigned int ANIMATION_FPS = 5;

Game::Game(){

}

void Game::init()
{
	//create player
	m_player = m_world.createEntity();

	//add systems to world
	m_world.addSystem(m_spriteRenderingSystem);
	m_world.addSystem(m_animationSystem);
	m_world.addSystem(m_playerInputSystem);
	m_world.addSystem(m_movementSystem);
	m_world.addSystem(m_collisionSystem);
	m_world.addSystem(m_lightingSystem);
	m_world.addSystem(m_physicsSystem);

	//add this world to appropriate listener lists
	m_playerInputSystem.addListener(this);
	m_collisionSystem.addListener(*this);

	m_animationSystem.setFps(ANIMATION_FPS);

	m_physicsSystem.setGravity(cp::Vect(0, 50));
}

void Game::load(sf::View screen){
	TexturesManager* manager = new TexturesManager;

    auto& playerSprite = m_player.addComponent<SpriteComponent>().sprite;

    playerSprite.setTexture(manager->getTexture("resources/textures/playerSpriteSheet"));

    if(!m_player.addComponent<AnimationComponent>().loadData("resources/meta/playerSpriteSheetFrames.txt"))
    {
        std::cerr << "Failed to load animation data\n";
        quit();
    }

    auto& playerAnimation = m_player.getComponent<AnimationComponent>();
    auto& playerTransform = m_player.addComponent<TransformComponent>().transform;

 //   auto& playerCollision = m_player.addComponent<PhysicsComponent>();
   // playerCollision.causesEvents = true;
    // NOTE: should have this in a file, but this'll do for now
    playerSprite.setOrigin(playerAnimation.frameSize.x / 2, playerAnimation.frameSize.y / 2);
    {
        constexpr int COLLISION_WIDTH = 32;
        constexpr int COLLISION_HEIGHT = 32;

    //    playerCollision.boundingBox = { (playerAnimation.frameSize.x - COLLISION_WIDTH) / 2 - playerSprite.getOrigin().x, 
      //                                  (playerAnimation.frameSize.y - COLLISION_HEIGHT) / 2 - playerSprite.getOrigin().y, 
     //                                   (float)COLLISION_WIDTH, (float)COLLISION_HEIGHT };
    }

    //set player position to be in middle of screen
    playerTransform.setPosition(screen.getSize().x / 2 - playerAnimation.frameSize.x / 2, screen.getSize().y / 2 - playerAnimation.frameSize.y / 2);

    m_player.addComponent<VelocityComponent>();
    auto& playerComp = m_player.addComponent<PlayerComponent>();
    playerComp.baseSpeed = 100;

   // m_wall = m_world.createEntity();

    // get wall sprite
   // auto& wallSprite = m_wall.addComponent<SpriteComponent>().sprite;
  //  wallSprite.setTexture(manager->getTexture("resources/textures/wall"));

   /* auto& wallTransform = m_wall.addComponent<TransformComponent>().transform;

    auto& wallCollision = m_wall.addComponent<PhysicComponent>();
   // wallCollision.causesEvents = false;
    //wallCollision.boundingBox = { { 0, 0 }, { wallSprite.getLocalBounds().width, wallSprite.getLocalBounds().height} };

    wallTransform.setPosition(screen.getSize().x * 0.75 - wallSprite.getLocalBounds().width / 2, 
                             (screen.getSize().y / 2 - playerAnimation.frameSize.y / 2) - wallCollision.boundingBox.height / 2);
*/
//	auto& vel = m_wall.addComponent<VelocityComponent>().velocity;
//	vel.x = 100;// , -100);

	auto& vec = m_world.createEntities(20);

	std::random_device rd;
	std::mt19937 gen(rd());

	for (auto& e : vec) {

		// get wall sprite
		auto& ws = e.addComponent<SpriteComponent>().sprite;
		ws = manager->getTexture("resources/textures/wall").getSprite();
		//ws.setTexture(*manager->getTexture("resources/textures/wall").get());
		ws.setOrigin(rectHalfDims<float>(ws.getTextureRect()));

		auto& wt = e.addComponent<TransformComponent>().transform;
		
		//wt.setPosition(screen.getSize().x * std::generate_canonical<float, 10>(gen), screen.getSize().y * std::generate_canonical<float, 10>(gen));
		wt.setPosition({ 320, 240 + std::generate_canonical<float, 10>(gen) });
	//	auto& v = e.addComponent<VelocityComponent>().velocity;
	//	v.x = 75 * (std::generate_canonical<float, 10>(gen) - 0.5);
		//v.y = 75 * (std::generate_canonical<float, 10>(gen) - 0.5);

		e.addComponent<LightComponent>();

		auto& physic = e.addComponent<PhysicsComponent>();
		physic.createBody(50, cpMomentForBox(50, 48, 48));
		auto box = physic.addBoxShape(48, 48, 0.2f);
		box->setElasticity(0.3);
		box->setFriction(0.8);

		e.activate();
	} 
	
	{
		auto& e = m_world.createEntity();
		auto& ws = e.addComponent<SpriteComponent>().sprite;
		ws = manager->getTexture("resources/textures/wall").getSprite();
		ws.setOrigin(rectHalfDims<float>(ws.getTextureRect()));

		auto& wt = e.addComponent<TransformComponent>().transform;
		wt.setPosition({ 320, 240 });
		wt.setRotation(0);

		e.addComponent<LightComponent>();

		auto& physic = e.addComponent<PhysicsComponent>();
	//	auto box = physic.addBoxShape(48, 48, 0);
		auto box = physic.addSegmentShape(cp::Vect(0, 400), cp::Vect(screen.getSize().x, 400), 0);
		//a->setElasticity(1); a->setFriction(1);
		box->setElasticity(0.3);
		box->setFriction(0.8);

		e.activate();
	}

	{
		auto& walls = m_world.createEntity();
		walls.addComponent<TransformComponent>().transform.setPosition(0, 0);

		auto& physic = walls.addComponent<PhysicsComponent>();
		auto a = physic.addSegmentShape(cp::Vect(0, 0), cp::Vect(0, screen.getSize().y), 0);
		a->setElasticity(1); a->setFriction(1);
		a = physic.addSegmentShape(cp::Vect(0, 0), cp::Vect(screen.getSize().x, 0), 0);
		a->setElasticity(1); a->setFriction(1);
		a = physic.addSegmentShape(cp::Vect(screen.getSize().x, 0), cp::Vect(screen.getSize().x, screen.getSize().y), 0);
		a->setElasticity(1); a->setFriction(1);
		a = physic.addSegmentShape(cp::Vect(0, screen.getSize().y), cp::Vect(screen.getSize().x, screen.getSize().y), 0);
		a->setElasticity(1); a->setFriction(1);

		walls.activate();
	}

//	auto& v = m_wall.getComponent<VelocityComponent>();

   // m_player.activate();
 //   m_wall.activate();

	manager->getTexture("resources/pointLightTexture").setSmooth(true);

	auto light = m_world.createEntity();
	auto& l = light.addComponent<LightComponent>() = LightComponent::pointLight(manager->getTexture("resources/pointLightTexture"), { 255, 0, 0 });
	l.texture.centerOrigin().setScale({ 18, 18 });
	light.addComponent<TransformComponent>().transform.setPosition(320, 100);
	light.activate();

	auto light2 = m_world.createEntity();
	auto& l2 = light2.addComponent<LightComponent>() = LightComponent::pointLight(manager->getTexture("resources/pointLightTexture"), { 0, 0, 255 });
	l2.texture.centerOrigin().setScale({ 15, 15 });
	light2.addComponent<TransformComponent>().transform.setPosition(180, 240);
	light2.activate();

	auto light3 = m_world.createEntity();
	auto& l3 = light3.addComponent<LightComponent>() = LightComponent::pointLight(manager->getTexture("resources/pointLightTexture"), { 0, 255, 0 });
	l3.texture.centerOrigin().setScale({ 15, 15 });
	light3.addComponent<TransformComponent>().transform.setPosition(500, 200);
	light3.activate();
}

void Game::update(float deltaTime){
    m_world.refresh();

    m_collisionSystem.update(deltaTime);	
    m_animationSystem.update(deltaTime);
    m_playerInputSystem.update(deltaTime);
	m_movementSystem.update(deltaTime);
	m_physicsSystem.update(deltaTime);
	m_lightingSystem.update(deltaTime);
}

void Game::render(sf::RenderTarget& renderTarget){
    // prepare screen for redraw
	renderTarget.clear(CLEAR_COLOR);

    // render all entities
	m_spriteRenderingSystem.render(renderTarget);
	m_lightingSystem.render(renderTarget);
}

void Game::handleEvents(sf::Event event)
{
    switch(event.type)
    {
        case sf::Event::Closed:
            quit();
            break;
        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
                case sf::Keyboard::Key::Escape:
                    quit();
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

void Game::onPlayerStateChanged(anax::Entity& e, PlayerComponent::State state)
{
    static const std::string stateNames[] = { "idle", "run", "run", "shoot_run", "shoot_run", "jump", "shoot", "shoot_jump" };

    auto& sprite = e.getComponent<SpriteComponent>().sprite;

    if(e.hasComponent<AnimationComponent>())
    {
        auto& animationComp = e.getComponent<AnimationComponent>();
        auto& stateName = stateNames[static_cast<unsigned>(state)];

        animationComp.play(stateName);  
        if(state == PlayerComponent::State::JUMP || state == PlayerComponent::State::JUMP_SHOOT || state == PlayerComponent::State::SHOOT)
        {
            // don't repeat animation for jumping
            // or shooting
            animationComp.repeat = false; 
        }
        else
        {
            animationComp.repeat = true;
        }
    }

    switch(state)
    {
        case PlayerComponent::State::MOVE_LEFT:
        case PlayerComponent::State::MOVE_LEFT_SHOOT:
            sprite.setScale(1, 1);
            break;
        case PlayerComponent::State::MOVE_RIGHT:
        case PlayerComponent::State::MOVE_RIGHT_SHOOT:
            sprite.setScale(-1, 1);
            break;
        default:
            break;
    }
}

void Game::onCollisionOccured(anax::Entity& e1, anax::Entity& e2)
{
    if(e1 != m_player && e2 != m_player)
    {
        return;
    }

    auto& velocity = m_player.getComponent<VelocityComponent>().velocity;
    auto& transform = m_player.getComponent<TransformComponent>().transform;
    transform.move(-velocity);
}
