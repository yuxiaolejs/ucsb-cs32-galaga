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

        bool outOfBound();

        virtual void tick() = 0;
    };

    class StupidProjectile : public Projectile
    {
    public:
        StupidProjectile();
        StupidProjectile(Vec2 position, bool isAlly);

        void tick();
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

        void spawnStupidProjectile(Vec2 position, bool isAlly);

        bool collisionDetection(Shape &shape, bool isAlly);

    private:
        std::vector<Projectile *> projectiles;
        Layer *targetLayer;
    };
}
#endif