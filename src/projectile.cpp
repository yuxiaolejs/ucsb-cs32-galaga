#include "lib/game.hpp"
#include "lib/shape.hpp"
#include "lib/layer.hpp"
#include "vec.hpp"
#include "projectile.hpp"
#include "lib/texture.hpp"
#include <mutex>
#include <thread>
#include <iostream>
#include <sstream>

using game::RES::Layer;
using game::RES::Shape;
using game::vec::Vec2;

using namespace game::projectile;

bool Projectile::outOfBound()
{
    return position.x > 10 || position.x < -10 || position.y > 6 || position.y < -6;
}

StupidProjectile::StupidProjectile() : Projectile(){};

StupidProjectile::StupidProjectile(Vec2 position, bool isAlly) : Projectile()
{
    this->position = position;
    this->isAlly = isAlly;
    if (isAlly)
        this->velocity = Vec2(0.1, 0);
    else
        this->velocity = Vec2(-0.1, 0);
}

StupidProjectile::StupidProjectile(Vec2 position, bool isAlly, Vec2 velocity) : Projectile()
{
    this->position = position;
    this->isAlly = isAlly;
    this->velocity = velocity;
}

void StupidProjectile::tick()
{
    ttl--;
    position = position + velocity;
}

SmartProjectile::SmartProjectile() : Projectile() {}

SmartProjectile::SmartProjectile(Vec2 position, bool isAlly, u_int32_t target, Layer *targetLayer) : Projectile()
{
    this->position = position;
    this->isAlly = isAlly;
    this->targetLayer = targetLayer;
    this->target = target;
    this->ttl = 400;    // plus explosion duration
}

void SmartProjectile::tick()
{
    ttl--;
    position = position + velocity;
    if (!targetLayer)
    {
        velocity = Vec2(0, -0.1);
        return;
    }
    Shape *_target = nullptr;
    for (size_t i = 0; i < targetLayer->shapes.size(); i++)
    {
        if (targetLayer->shapes[i].id == target)
        {
            _target = &(targetLayer->shapes[i]);
            break;
        }
    }
    if (!_target)
    {
        velocity = Vec2(0, 0.1);
        return;
    }

    const float KP = 0.005;
    const float KI = 0.0015;
    const float KD = 0.002;
    const float INTEGRAL_LIMIT = 50.0;

    Vec2 targetPosition = Vec2(_target->x, _target->y);
    // Use PID algorithm to calculate velocity
    Vec2 P = targetPosition - position;
    Vec2 D = P - prevDiff;
    Vec2 I = intgDiff + P / 10.00;

    // Limit integral
    if (I.magnitude() > INTEGRAL_LIMIT) {
        I = I / I.magnitude() * INTEGRAL_LIMIT;
    }

    velocity = P * KP + I * KI + D * KD;
    prevDiff = P;
    intgDiff = I;
}

ProjectileManager::ProjectileManager(Layer *targetLayer) : targetLayer(targetLayer){};

ProjectileManager::ProjectileManager() : targetLayer(nullptr){};

ProjectileManager::~ProjectileManager()
{
    for (size_t i = 0; i < projectiles.size(); i++)
        delete projectiles[i];
}

void ProjectileManager::tick()
{
    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    const int explosionDuration = 40;

    for (size_t i = 0; i < projectiles.size(); i++)
    {
        projectiles[i]->tick();
        if (projectiles[i]->outOfBound() || projectiles[i]->ttl == 0)
        {
            deleteProjectile(i);
            i--;
        }
        else if (projectiles[i]->ttl <= explosionDuration)
        {
            if (projectiles[i]->ttl < explosionDuration)
                continue;
            updateProjectileTexture(i, "explode", 0.5, 0.5, 45);
        }
        else
        {
            updateProjectile(i);
        }
    }
    delete layersLock;
}

void ProjectileManager::deleteProjectile(size_t idx)
{
    if (idx >= projectiles.size())
        return;
    delete projectiles[idx];
    projectiles.erase(projectiles.begin() + idx);
    targetLayer->shapes.erase(targetLayer->shapes.begin() + idx);
}

void ProjectileManager::updateProjectile(size_t idx)
{
    if (!targetLayer || idx >= projectiles.size() || idx >= targetLayer->shapes.size())
        return;
    targetLayer->shapes[idx].x = projectiles[idx]->position.x;
    targetLayer->shapes[idx].y = projectiles[idx]->position.y;
    targetLayer->shapes[idx].rotation = projectiles[idx]->velocity.getAngleDeg() - 90;
}

void ProjectileManager::updateProjectileTexture(size_t idx, std::string textureName, float width, float height, int rotation)
{
    if (!targetLayer || idx >= projectiles.size() || idx >= targetLayer->shapes.size())
        return;
    targetLayer->shapes[idx].texture = game::textureManager.getTexture(textureName);
    targetLayer->shapes[idx].width = width;
    targetLayer->shapes[idx].height = height;
    targetLayer->shapes[idx].rotation = rotation;
}

void ProjectileManager::spawnStupidProjectile(Vec2 position, bool isAlly)
{
    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    projectiles.push_back(new StupidProjectile(position, isAlly));
    Shape shape;
    shape.texture = game::textureManager.getTexture("bullet_2");
    shape.x = position.x;
    shape.y = position.y;
    shape.width = 0.15;
    shape.height = 0.29;
    shape.rotation = 0;
    targetLayer->shapes.push_back(shape);
    delete layersLock;
}

void ProjectileManager::spawnStupidProjectile(Vec2 position, bool isAlly, Vec2 velocity)
{
    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    projectiles.push_back(new StupidProjectile(position, isAlly, velocity));
    Shape shape;
    shape.texture = game::textureManager.getTexture("bullet_1");
    shape.x = position.x;
    shape.y = position.y;
    shape.width = 0.2;
    shape.height = 0.2;
    shape.rotation = 0;
    targetLayer->shapes.push_back(shape);
    delete layersLock;
}

void ProjectileManager::spawnSmartProjectile(Vec2 position, bool isAlly, u_int32_t target, Layer *_targetLayer)
{
    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    projectiles.push_back(new SmartProjectile(position, isAlly, target, _targetLayer));
    Shape shape;
    shape.texture = game::textureManager.getTexture("missile_2");
    shape.x = position.x;
    shape.y = position.y;
    shape.width = 0.4;
    shape.height = 0.65;
    shape.rotation = 0;
    targetLayer->shapes.push_back(shape);
    delete layersLock;
}

bool ProjectileManager::collisionDetection(Shape &shape, bool isAlly)
{
    for (size_t i = 0; i < projectiles.size(); i++)
    {
        if (projectiles[i]->isAlly == isAlly)
            continue;
        if (projectiles[i]->position.x < shape.x + shape.width / 2 &&
            projectiles[i]->position.x > shape.x - shape.width / 2 &&
            projectiles[i]->position.y < shape.y + shape.height / 2 &&
            projectiles[i]->position.y > shape.y - shape.height / 2)
        {
            std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
            deleteProjectile(i);
            delete layersLock;
            return true;
        }
    }
    return false;
}