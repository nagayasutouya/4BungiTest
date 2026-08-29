#ifndef COLOR_H_
#define COLOR_H_

// RGBAƒJƒ‰[
struct Color {
    // Ô¬•ª (0.0f ` 1.0f)
    float r{ 0.0f };
    // —Î¬•ª (0.0f ` 1.0f)
    float g{ 0.0f };
    // Â¬•ª (0.0f ` 1.0f)
    float b{ 0.0f };
    // ƒAƒ‹ƒtƒ@¬•ª (0.0f ` 1.0f)
    float a{ 1.0f };
    //‚Å‚Ó‚§‚ğ•Ô‚·
    static Color one() { return Color{1.0f,1.0f,1.0f,1.0f}; }
};

#endif