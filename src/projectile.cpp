#include "lib/game.hpp"
#include "lib/shape.hpp"
#include "lib/layer.hpp"
#include "vec.hpp"
#include "projectile.hpp"
#include "lib/texture.hpp"
#include <mutex>
#include <thread>

using game::RES::Layer;
using game::RES::Shape;
using game::vec::Vec2;

using namespace game::projectile;

bool Projectile::outOfBound()
{
    return position.x > 10 || position.x < -10 || position.y > 5 || position.y < -5;
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
    position = position + velocity;
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
    for (size_t i = 0; i < projectiles.size(); i++)
    {
        projectiles[i]->tick();
        if (projectiles[i]->outOfBound())
        {
            deleteProjectile(i);
            i--;
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
    targetLayer->shapes[idx].rotation = projectiles[idx]->velocity.getAngleDeg();
}

void ProjectileManager::spawnStupidProjectile(Vec2 position, bool isAlly)
{
    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    projectiles.push_back(new StupidProjectile(position, isAlly));
    Shape shape;
    shape.texture = game::textureManager.getTexture("proj1");
    shape.x = position.x;
    shape.y = position.y;
    shape.width = 0.5;
    shape.height = 0.5;
    shape.rotation = 0;
    targetLayer->shapes.push_back(shape);
    delete layersLock;
}

void ProjectileManager::spawnStupidProjectile(Vec2 position, bool isAlly, Vec2 velocity)
{
    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    projectiles.push_back(new StupidProjectile(position, isAlly, velocity));
    Shape shape;
    shape.texture = game::textureManager.getTexture("proj1");
    shape.x = position.x;
    shape.y = position.y;
    shape.width = 0.5;
    shape.height = 0.5;
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