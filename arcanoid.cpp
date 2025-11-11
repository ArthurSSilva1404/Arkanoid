/**
 * @file Arkanoid.cpp
 * @author Adaptado para C++
 * @brief Arkanoid game in C++ using Raylib (https://www.raylib.com/)
 */

#include <raylib.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <memory>

constexpr int TARGET_HORIZONTAL_MAX_QUANTITY = 11;
constexpr int TARGET_VERTICAL_MAX_QUANTITY = 22;
constexpr int BALL_COLLISION_PROBES_QUANTITY = 12;

constexpr int TARGET_WIDTH = 64;
constexpr int TARGET_HEIGHT = 32;
constexpr int PLAYER_AREA_HEIGHT = 160;
constexpr int PLAYER_WIDTH = 128;
constexpr int PLAYER_HEIGHT = 32;
constexpr int BALL_RADIUS = 10;
constexpr int GAME_STATUS_WIDTH = 224;
constexpr int ARENA_MARGIN = 32;
constexpr Color OVERLAY_COLOR = {0, 0, 0, 200};
constexpr bool DRAW_COLLISION_PROBES = false;

enum class GameState { IDLE, PLAYING, WON, LOSE };
enum class CollisionType { LEFT, RIGHT, TOP, DOWN, NONE };

struct PolarCoord {
    float angle{};
    float distance{};
};

struct Player {
    Rectangle rec{};
    Vector2 vel{};
    Color color{};
};

struct TargetType {
    Color color{};
    int points{};
    TargetType* next{nullptr};
};

struct Target {
    Rectangle rec{};
    TargetType* type{nullptr};
    bool active{true};
};

struct Ball {
    Vector2 pos{};
    Vector2 vel{};
    int radius{};
    Color color{};
    PolarCoord collisionProbes[BALL_COLLISION_PROBES_QUANTITY]{};
};

struct Score {
    Vector2 pos{};
    int points{};
};

struct Arena {
    Rectangle rec{};
    Rectangle battleField{};
    std::shared_ptr<Player> player;
    std::shared_ptr<Ball> ball;
    std::vector<std::vector<Target>> targets;
    int targetLines{};
    int targetColumns{};
    int targetQuantity{};
    int inactiveTargets{};
    Color color{};
    Color battleFieldColor{};
};

struct GameWorld {
    std::shared_ptr<Arena> arena;
    std::shared_ptr<Score> score;
    GameState state{GameState::IDLE};
};

inline float toRadians(float deg) {
    return deg * (PI / 180.0f);
}

CollisionType checkCollisionBallPlayer(const Ball& ball, const Player& player) {
    CollisionType cTypes[4] = {CollisionType::RIGHT, CollisionType::DOWN, CollisionType::LEFT, CollisionType::TOP};
    for (int i = 0; i < BALL_COLLISION_PROBES_QUANTITY; i++) {
        const auto& pc = ball.collisionProbes[i];
        float x = ball.pos.x + pc.distance * cos(toRadians(pc.angle));
        float y = ball.pos.y + pc.distance * sin(toRadians(pc.angle));
        if (CheckCollisionPointRec({x, y}, player.rec))
            return cTypes[(i + 1) % 4];
    }
    return CollisionType::NONE;
}

CollisionType checkCollisionBallTarget(const Ball& ball, const Target& target) {
    CollisionType cTypes[4] = {CollisionType::RIGHT, CollisionType::DOWN, CollisionType::LEFT, CollisionType::TOP};
    for (int i = 0; i < BALL_COLLISION_PROBES_QUANTITY; i++) {
        const auto& pc = ball.collisionProbes[i];
        float x = ball.pos.x + pc.distance * cos(toRadians(pc.angle));
        float y = ball.pos.y + pc.distance * sin(toRadians(pc.angle));
        if (CheckCollisionPointRec({x, y}, target.rec))
            return cTypes[(i + 1) % 4];
    }
    return CollisionType::NONE;
}

std::shared_ptr<GameWorld> createGameWorld() {
    auto gw = std::make_shared<GameWorld>();
    auto arena = std::make_shared<Arena>();
    auto player = std::make_shared<Player>();
    auto ball = std::make_shared<Ball>();
    auto score = std::make_shared<Score>();

    int arenaWidth = TARGET_WIDTH * TARGET_HORIZONTAL_MAX_QUANTITY;
    int arenaHeight = TARGET_HEIGHT * TARGET_VERTICAL_MAX_QUANTITY;
    int vOffsetTargets = 4 * TARGET_HEIGHT;

    static TargetType tt6{{162, 160, 162, 255}, 10, nullptr};
    static TargetType tt5{{226, 57, 0, 255}, 10, &tt6};
    static TargetType tt4{{0, 84, 246, 255}, 10, &tt5};
    static TargetType tt3{{255, 149, 1, 255}, 10, &tt4};
    static TargetType tt2{{255, 119, 145, 255}, 10, &tt3};
    static TargetType tt1{{116, 183, 9, 255}, 10, &tt2};
    TargetType* targetTypes[6] = {&tt1, &tt2, &tt3, &tt4, &tt5, &tt6};

    arena->rec = {0, 0, float(arenaWidth + ARENA_MARGIN * 2), float(arenaHeight + ARENA_MARGIN)};
    arena->battleField = {ARENA_MARGIN, ARENA_MARGIN, float(arenaWidth), float(arenaHeight)};
    arena->targetLines = 6;
    arena->targetColumns = 11;
    arena->targetQuantity = 6 * 11;
    arena->inactiveTargets = 0;
    arena->color = GRAY;
    arena->battleFieldColor = DARKGRAY;

    player->rec = {
        arena->battleField.x + arena->battleField.width / 2 - PLAYER_WIDTH / 2,
        arena->battleField.height - PLAYER_AREA_HEIGHT + PLAYER_HEIGHT,
        PLAYER_WIDTH,
        PLAYER_HEIGHT};
    player->vel = {5, 0};
    player->color = WHITE;

    ball->pos = {arena->battleField.x + arena->battleField.width / 2, player->rec.y - BALL_RADIUS};
    ball->vel = {5, -5};
    ball->radius = BALL_RADIUS;
    ball->color = ORANGE;

    float tick = 360.0f / BALL_COLLISION_PROBES_QUANTITY;
    for (int i = 0; i < BALL_COLLISION_PROBES_QUANTITY; i++)
        ball->collisionProbes[i] = {i * tick, (float)BALL_RADIUS};

    arena->player = player;
    arena->ball = ball;
    arena->targets.resize(arena->targetLines, std::vector<Target>(arena->targetColumns));

    for (int i = 0; i < arena->targetLines; i++)
        for (int j = 0; j < arena->targetColumns; j++)
            arena->targets[i][j] = {{arena->battleField.x + j * TARGET_WIDTH,
                                     vOffsetTargets + arena->battleField.y + i * TARGET_HEIGHT,
                                     TARGET_WIDTH,
                                     TARGET_HEIGHT},
                                    targetTypes[i],
                                    true};

    score->pos = {arena->rec.x + arena->rec.width + 20, arena->rec.y + 20};
    score->points = 0;

    gw->arena = arena;
    gw->score = score;
    gw->state = GameState::IDLE;

    return gw;
}

void drawArena(const Arena& arena) {
    DrawRectangleRec(arena.rec, arena.color);
    DrawRectangleRec(arena.battleField, arena.battleFieldColor);

    for (const auto& row : arena.targets)
        for (const auto& t : row)
            if (t.active) {
                DrawRectangle(t.rec.x, t.rec.y, t.rec.width - 4, t.rec.height - 4, BLACK);
                DrawRectangle(t.rec.x, t.rec.y, t.rec.width - 6, t.rec.height - 6, t.type->color);
            }

    DrawRectangleRec(arena.player->rec, arena.player->color);
    DrawCircleV(arena.ball->pos, arena.ball->radius, arena.ball->color);
}

void drawScore(const Score& score) {
    DrawText("SCORE", score.pos.x, score.pos.y, 40, RED);
    DrawText(TextFormat("%05d", score.points), score.pos.x, score.pos.y + 40, 40, WHITE);
}

void inputAndUpdate(GameWorld& gw) {
    auto& arena = *gw.arena;
    auto& player = *arena.player;
    auto& ball = *arena.ball;
    auto& score = *gw.score;

    if (gw.state == GameState::IDLE) {
        if (GetKeyPressed() != 0) gw.state = GameState::PLAYING;
        return;
    }

    if (gw.state == GameState::WON || gw.state == GameState::LOSE) {
        if (GetKeyPressed() != 0) gw = *createGameWorld();
        return;
    }

    // Movement and collision logic
    ball.pos.x += ball.vel.x;
    ball.pos.y += ball.vel.y;

    if (IsKeyDown(KEY_LEFT))
        player.rec.x = std::max(player.rec.x - player.vel.x, arena.battleField.x);

    if (IsKeyDown(KEY_RIGHT))
        player.rec.x = std::min(player.rec.x + player.vel.x, arena.battleField.x + arena.battleField.width - player.rec.width);

    // Wall collisions
    if (ball.pos.x - ball.radius < arena.battleField.x || ball.pos.x + ball.radius > arena.battleField.x + arena.battleField.width)
        ball.vel.x *= -1;

    if (ball.pos.y - ball.radius < arena.battleField.y)
        ball.vel.y *= -1;
    else if (ball.pos.y - ball.radius > arena.battleField.y + arena.battleField.height)
        gw.state = GameState::LOSE;

    // Player collision
    if (checkCollisionBallPlayer(ball, player) != CollisionType::NONE)
        ball.vel.y *= -1;

    // Target collisions
    for (auto& row : arena.targets)
        for (auto& t : row)
            if (t.active) {
                if (checkCollisionBallTarget(ball, t) != CollisionType::NONE) {
                    score.points += t.type->points;
                    t.type = t.type->next;
                    if (t.type == nullptr) {
                        t.active = false;
                        arena.inactiveTargets++;
                    }
                    ball.vel.y *= -1;
                }
            }

    if (arena.inactiveTargets == arena.targetQuantity)
        gw.state = GameState::WON;
}

void draw(const GameWorld& gw) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawArena(*gw.arena);
    drawScore(*gw.score);

    if (gw.state != GameState::PLAYING)
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), OVERLAY_COLOR);

    const char* msg = nullptr;
    switch (gw.state) {
        case GameState::IDLE:
            msg = "Tecle algo para começar.";
            break;
        case GameState::WON:
            msg = "Você ganhou! Tecle algo para recomeçar.";
            break;
        case GameState::LOSE:
            msg = "Você perdeu... Tecle algo para recomeçar.";
            break;
        default:
            break;
    }

    if (msg) {
        int w = MeasureText(msg, 40);
        DrawText(msg, GetScreenWidth() / 2 - w / 2, GetScreenHeight() / 2 - 40, 40, WHITE);
    }

    EndDrawing();
}

int main() {
    const int screenWidth = ARENA_MARGIN + TARGET_WIDTH * TARGET_HORIZONTAL_MAX_QUANTITY + ARENA_MARGIN + GAME_STATUS_WIDTH;
    const int screenHeight = ARENA_MARGIN + TARGET_HEIGHT * TARGET_VERTICAL_MAX_QUANTITY;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Arkanoid (C++)");
    InitAudioDevice();
    SetTargetFPS(60);

    auto gw = createGameWorld();

    while (!WindowShouldClose()) {
        inputAndUpdate(*gw);
        draw(*gw);
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

