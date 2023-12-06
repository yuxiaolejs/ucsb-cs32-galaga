#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include "lib/game.hpp"
#include "lib/shape.hpp"
#include "lib/layer.hpp"
#include "vec.hpp"
using game::RES::Layer;
using game::RES::Shape;
using game::vec::Vec2;
namespace game::projectile
{
    class Projectile
    {
    public:
        Vec2 position;
        Vec2 velocity;
        bool isAlly;
        size_t ttl = 1000;

        bool outOfBound();

        virtual void tick() = 0;
    };

    class StupidProjectile : public Projectile
    {
    public:
        StupidProjectile();
        StupidProjectile(Vec2 position, bool isAlly);
        StupidProjectile(Vec2 position, bool isAlly, Vec2 velocity);

        void tick();
    };

    class SmartProjectile : public Projectile
    {
    public:
        SmartProjectile();
        SmartProjectile(Vec2 position, bool isAlly, u_int32_t target, Layer *targetLayer);

        Vec2 prevDiff;
        Vec2 intgDiff;

        void tick();

    private:
        u_int32_t target;
        Layer *targetLayer;
    };

    class ProjectileManager
    {
    public:
        ProjectileManager(Layer *targetLayer);
        ProjectileManager();
        ~ProjectileManager();

        void tick();
        void deleteProjectile(size_t idx);
        void updateProjectile(size_t idx);
        void updateProjectileTexture(size_t idx, std::string textureName, float width, float height, int rotation);

        void spawnStupidProjectile(Vec2 position, bool isAlly);
        void spawnStupidProjectile(Vec2 position, bool isAlly, Vec2 velocity);

        void spawnSmartProjectile(Vec2 position, bool isAlly, u_int32_t target, Layer *_targetLayer);

        bool collisionDetection(Shape &shape, bool isAlly);

    private:
        std::vector<Projectile *> projectiles;
        Layer *targetLayer;
    };
}
#endif