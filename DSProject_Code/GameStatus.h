#pragma once

// Possible results of the game (used by Battle and FileManager).
enum GameStatus
{
    GAME_WIN,     // all enemies are killed
    GAME_LOSS,    // all towers are destroyed
    GAME_DRAWN    // anything else
};
