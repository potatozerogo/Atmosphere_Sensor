/*
 * Table of ASCII[0x20-0x7F] and CP1251[0xA0-0xFF]
 */
static unsigned char const Font7x11[] ={
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // space 0x20  32
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x08, 0x08,   // ! 0x21  33
    0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // " 0x22  34
    0x48, 0x48, 0x24, 0x7f, 0x24, 0x24, 0x12, 0x7f, 0x12, 0x09, 0x09,   // # 0x23  35
    0x08, 0x3e, 0x49, 0x09, 0x09, 0x3e, 0x48, 0x48, 0x49, 0x3e, 0x08,   // $ 0x24  36
    0x47, 0x25, 0x27, 0x10, 0x10, 0x08, 0x08, 0x04, 0x72, 0x52, 0x71,   // % 0x25  37
    0x06, 0x09, 0x09, 0x09, 0x26, 0x22, 0x25, 0x29, 0x11, 0x29, 0x46,   // & 0x26  38
    0x1c, 0x22, 0x22, 0x22, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // ° // ' 0x27  39 为了显示℃而修改(2021.2.20)
    0x08, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x08,   // ( 0x28  40
    0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04,   // ) 0x29  41
    0x00, 0x08, 0x2a, 0x1c, 0x2a, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,   // * 0x2A  42
    0x00, 0x00, 0x00, 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00, 0x00, 0x00,   // + 0x2B  43
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0c, 0x04,   // , 0x2C  44
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,   // - 0x2D  45
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18,   // . 0x2E  46
    0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x02, 0x02,   // / 0x2F  47
    0x1c, 0x22, 0x51, 0x51, 0x49, 0x49, 0x49, 0x45, 0x45, 0x22, 0x1c,   // 0 0x30  48
    0x08, 0x0c, 0x0a, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e,   // 1 0x31  49
    0x1e, 0x21, 0x40, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x7f,   // 2 0x32  50
    0x1f, 0x20, 0x40, 0x40, 0x20, 0x1e, 0x20, 0x40, 0x40, 0x20, 0x1f,   // 3 0x33  51
    0x30, 0x28, 0x24, 0x22, 0x21, 0x7f, 0x20, 0x20, 0x20, 0x20, 0x20,   // 4 0x34  52
    0x7f, 0x01, 0x01, 0x01, 0x1f, 0x20, 0x40, 0x40, 0x40, 0x20, 0x1f,   // 5 0x35  53
    0x38, 0x04, 0x02, 0x01, 0x1d, 0x23, 0x41, 0x41, 0x41, 0x22, 0x1c,   // 6 0x36  54
    0x7f, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02,   // 7 0x37  55
    0x1c, 0x22, 0x41, 0x41, 0x22, 0x1c, 0x22, 0x41, 0x41, 0x22, 0x1c,   // 8 0x38  56
    0x1c, 0x22, 0x41, 0x41, 0x41, 0x62, 0x5c, 0x40, 0x20, 0x10, 0x0e,   // 9 0x39  57
    0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00,   // : 0x3A  58
    0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x08, 0x04,   // ; 0x3B  59
    0x00, 0x40, 0x30, 0x0c, 0x02, 0x01, 0x02, 0x0c, 0x30, 0x40, 0x00,   // < 0x3C  60
    0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00,   // = 0x3D  61
    0x00, 0x01, 0x06, 0x18, 0x20, 0x40, 0x20, 0x18, 0x06, 0x01, 0x00,   // > 0x3E  62
    0x3e, 0x41, 0x40, 0x40, 0x30, 0x08, 0x08, 0x08, 0x00, 0x08, 0x08,   // ? 0x3F  63
    0x1c, 0x22, 0x41, 0x49, 0x55, 0x55, 0x55, 0x39, 0x01, 0x02, 0x3c,   // @ 0x40  64
    0x08, 0x14, 0x14, 0x22, 0x22, 0x41, 0x41, 0x7f, 0x41, 0x41, 0x41,   // A 0x41  65
    0x1f, 0x21, 0x21, 0x21, 0x1f, 0x21, 0x41, 0x41, 0x41, 0x21, 0x1f,   // B 0x42  66
    0x3c, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x3c,   // C 0x43  67
    0x1f, 0x22, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1f,   // D 0x44  68
    0x7f, 0x01, 0x01, 0x01, 0x1f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7f,   // E 0x45  69
    0x7f, 0x01, 0x01, 0x01, 0x3f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   // F 0x46  70
    0x3c, 0x42, 0x01, 0x01, 0x01, 0x79, 0x41, 0x41, 0x41, 0x42, 0x3c,   // G 0x47  71
    0x41, 0x41, 0x41, 0x41, 0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,   // H 0x48  72
    0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c,   // I 0x49  73
    0x1e, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0e,   // J 0x4A  74
    0x21, 0x11, 0x09, 0x05, 0x03, 0x03, 0x05, 0x09, 0x11, 0x21, 0x41,   // K 0x4B  75
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7f,   // L 0x4C  76
    0x41, 0x63, 0x55, 0x55, 0x49, 0x49, 0x41, 0x41, 0x41, 0x41, 0x41,   // M 0x4D  77
    0x41, 0x41, 0x41, 0x43, 0x43, 0x45, 0x49, 0x51, 0x51, 0x61, 0x41,   // N 0x4E  78
    0x1c, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c,   // O 0x4F  79
    0x1f, 0x21, 0x41, 0x41, 0x41, 0x21, 0x1f, 0x01, 0x01, 0x01, 0x01,   // P 0x50  80
    0x1c, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x49, 0x51, 0x22, 0x5c,   // Q 0x51  81
    0x1f, 0x21, 0x41, 0x41, 0x41, 0x21, 0x1f, 0x09, 0x11, 0x21, 0x41,   // R 0x52  82
    0x1c, 0x22, 0x41, 0x01, 0x02, 0x1c, 0x20, 0x40, 0x41, 0x22, 0x1c,   // S 0x53  83
    0x7f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // T 0x54  84
    0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c,   // U 0x55  85
    0x41, 0x41, 0x22, 0x22, 0x22, 0x22, 0x14, 0x14, 0x14, 0x08, 0x08,   // V 0x56  86
    0x41, 0x41, 0x41, 0x41, 0x49, 0x49, 0x49, 0x55, 0x55, 0x63, 0x41,   // W 0x57  87
    0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x14, 0x22, 0x22, 0x41,   // X 0x58  88
    0x41, 0x41, 0x22, 0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // Y 0x59  89
    0x7f, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01, 0x7f,   // Z 0x5A  90
    0x1c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1c,   // [ 0x5B  91
    0x01, 0x02, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10,   // \ 0x5C  92
    0x0e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0e,   // ] 0x5D  93
    0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // ^ 0x5E  94
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,   // _ 0x5F  95
    0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // ` 0x60  96
    0x00, 0x00, 0x00, 0x3e, 0x41, 0x40, 0x7e, 0x41, 0x41, 0x61, 0x5e,   // a 0x61  97
    0x01, 0x01, 0x01, 0x1d, 0x23, 0x41, 0x41, 0x41, 0x41, 0x23, 0x1d,   // b 0x62  98
    0x00, 0x00, 0x00, 0x3c, 0x42, 0x01, 0x01, 0x01, 0x01, 0x42, 0x3c,   // c 0x63  99
    0x40, 0x40, 0x40, 0x5c, 0x62, 0x41, 0x41, 0x41, 0x41, 0x62, 0x5c,   // d 0x64 100
    0x00, 0x00, 0x00, 0x1c, 0x22, 0x41, 0x7f, 0x01, 0x01, 0x42, 0x3c,   // e 0x65 101
    0x1c, 0x04, 0x04, 0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,   // f 0x66 102
    0x00, 0x00, 0x00, 0x5e, 0x61, 0x41, 0x61, 0x5e, 0x40, 0x21, 0x1e,   // g 0x67 103
    0x01, 0x01, 0x01, 0x3d, 0x43, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,   // h 0x68 104
    0x08, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // i 0x69 105
    0x00, 0x10, 0x00, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x12, 0x0c,   // j 0x6A 106
    0x01, 0x01, 0x01, 0x21, 0x11, 0x09, 0x07, 0x09, 0x11, 0x21, 0x41,   // k 0x6B 107
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0c,   // l 0x6C 108
    0x00, 0x00, 0x00, 0x25, 0x5b, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49,   // m 0x6D 109
    0x00, 0x00, 0x00, 0x1d, 0x23, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,   // n 0x6E 110
    0x00, 0x00, 0x00, 0x1c, 0x22, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c,   // o 0x6F 111
    0x00, 0x00, 0x00, 0x1f, 0x21, 0x41, 0x41, 0x21, 0x1f, 0x01, 0x01,   // p 0x70 112
    0x00, 0x00, 0x00, 0x2c, 0x32, 0x21, 0x21, 0x21, 0x32, 0x2c, 0x20,   // q 0x71 113
    0x00, 0x00, 0x00, 0x3d, 0x43, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   // r 0x72 114
    0x00, 0x00, 0x00, 0x3e, 0x41, 0x01, 0x0e, 0x30, 0x40, 0x41, 0x3e,   // s 0x73 115
    0x00, 0x04, 0x04, 0x3e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x38,   // t 0x74 116
    0x00, 0x00, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x31, 0x2e,   // u 0x75 117
    0x00, 0x00, 0x00, 0x41, 0x41, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08,   // v 0x76 118
    0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x49, 0x49, 0x49, 0x49, 0x36,   // w 0x77 119
    0x00, 0x00, 0x00, 0x41, 0x22, 0x14, 0x08, 0x08, 0x14, 0x22, 0x41,   // x 0x78 120
    0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x24, 0x38, 0x10, 0x08, 0x06,   // y 0x79 121
    0x00, 0x00, 0x00, 0x7f, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x7f,   // z 0x7A 122
    0x18, 0x08, 0x08, 0x08, 0x04, 0x06, 0x04, 0x08, 0x08, 0x08, 0x18,   // { 0x7B 123
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // | 0x7C 124
    0x0c, 0x08, 0x08, 0x08, 0x10, 0x30, 0x10, 0x08, 0x08, 0x08, 0x0c,   // } 0x7D 125
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0a, 0x51, 0x20, 0x00, 0x00, 0x00,   // ~ 0x7E 126
    0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,   //  0x7F 127
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // (NBSP) 0xA0 160
    0x55, 0x49, 0x42, 0x22, 0x24, 0x24, 0x18, 0x10, 0x08, 0x04, 0x03,   // Ў 0xA1 161
    0x00, 0x14, 0x08, 0x41, 0x41, 0x42, 0x24, 0x38, 0x10, 0x08, 0x06,   // ў 0xA2 162
    0x1e, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0e,   // Ј 0xA3 163
    0x00, 0x00, 0x00, 0x21, 0x1e, 0x12, 0x12, 0x1e, 0x21, 0x00, 0x00,   // ¤ 0xA4 164
    0x40, 0x7f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   // Ґ 0xA5 165
    0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08,   // ¦ 0xA6 166
    0x10, 0x08, 0x04, 0x08, 0x14, 0x22, 0x14, 0x08, 0x10, 0x08, 0x04,   // § 0xA7 167
    0x14, 0x7f, 0x01, 0x01, 0x01, 0x1f, 0x01, 0x01, 0x01, 0x01, 0x7f,   // Ё 0xA8 168
    0x1c, 0x22, 0x41, 0x5d, 0x45, 0x45, 0x45, 0x5d, 0x41, 0x22, 0x1c,   // © 0xA9 169
    0x3c, 0x42, 0x01, 0x01, 0x01, 0x1f, 0x01, 0x01, 0x01, 0x42, 0x3c,   // Є 0xAA 170
    0x00, 0x00, 0x00, 0x48, 0x24, 0x12, 0x09, 0x12, 0x24, 0x48, 0x00,   // « 0xAB 171
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x40, 0x40, 0x00, 0x00, 0x00,   // ¬ 0xAC 172
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00,   // - (hyphen) 0xAD 173
    0x3e, 0x41, 0x41, 0x5d, 0x55, 0x4d, 0x55, 0x55, 0x41, 0x41, 0x3e,   // ® 0xAE 174
    0x14, 0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c,   // Ї 0xAF 175
    0x00, 0x00, 0x1c, 0x22, 0x22, 0x22, 0x1c, 0x00, 0x00, 0x00, 0x00,   // ° 0xB0 176
    0x00, 0x00, 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00, 0x3e, 0x00, 0x00,   // ± 0xB1 177
    0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c,   // І 0xB2 178
    0x08, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // і 0xB3 179
    0x00, 0x00, 0x20, 0x3f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   // ґ 0xB4 180
    0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x5e, 0x02, 0x01,   // µ 0xB5 181
    0x00, 0x7e, 0x27, 0x26, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24,   // ¶ 0xB6 182
    0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // · 0xB7 183
    0x00, 0x12, 0x00, 0x1c, 0x22, 0x41, 0x7f, 0x01, 0x01, 0x22, 0x1c,   // ё 0xB8 184
    0x70, 0x50, 0x70, 0x00, 0x43, 0x45, 0x49, 0x51, 0x61, 0x41, 0x41,   // № 0xB9 185
    0x00, 0x00, 0x00, 0x3c, 0x42, 0x01, 0x1f, 0x01, 0x01, 0x42, 0x3c,   // є 0xBA 186
    0x00, 0x00, 0x00, 0x09, 0x12, 0x24, 0x48, 0x24, 0x12, 0x09, 0x00,   // » 0xBB 187
    0x00, 0x10, 0x00, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x12, 0x0c,   // ј 0xBC 188
    0x1c, 0x22, 0x41, 0x01, 0x02, 0x1c, 0x20, 0x40, 0x41, 0x22, 0x1c,   // Ѕ 0xBD 189
    0x00, 0x00, 0x00, 0x3e, 0x41, 0x01, 0x0e, 0x30, 0x40, 0x41, 0x3e,   // ѕ 0xBE 190
    0x00, 0x14, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // ї 0xBF 191
    0x08, 0x14, 0x14, 0x22, 0x22, 0x41, 0x41, 0x7f, 0x41, 0x41, 0x41,   // А 0xC0 192
    0x7f, 0x01, 0x01, 0x01, 0x1f, 0x21, 0x41, 0x41, 0x41, 0x21, 0x1f,   // Б 0xC1 193
    0x1f, 0x21, 0x21, 0x21, 0x1f, 0x21, 0x41, 0x41, 0x41, 0x21, 0x1f,   // В 0xC2 194
    0x7f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   // Г 0xC3 195
    0x3e, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x7f,   // Д 0xC4 196
    0x7f, 0x01, 0x01, 0x01, 0x1f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7f,   // Е 0xC5 197
    0x49, 0x49, 0x49, 0x2a, 0x1c, 0x1c, 0x2a, 0x2a, 0x49, 0x49, 0x49,   // Ж 0xC6 198
    0x1e, 0x21, 0x40, 0x40, 0x20, 0x1e, 0x20, 0x40, 0x40, 0x21, 0x1e,   // З 0xC7 199
    0x41, 0x61, 0x51, 0x51, 0x49, 0x45, 0x45, 0x43, 0x41, 0x41, 0x41,   // И 0xC8 200
    0x4d, 0x61, 0x51, 0x51, 0x49, 0x45, 0x45, 0x43, 0x41, 0x41, 0x41,   // Й 0xC9 201
    0x61, 0x11, 0x09, 0x09, 0x05, 0x03, 0x05, 0x09, 0x11, 0x21, 0x41,   // К 0xCA 202
    0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x41,   // Л 0xCB 203
    0x41, 0x63, 0x55, 0x55, 0x49, 0x49, 0x41, 0x41, 0x41, 0x41, 0x41,   // М 0xCC 204
    0x41, 0x41, 0x41, 0x41, 0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,   // Н 0xCD 205
    0x1c, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c,   // О 0xCE 206
    0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,   // П 0xCF 207
    0x1f, 0x21, 0x41, 0x41, 0x41, 0x21, 0x1f, 0x01, 0x01, 0x01, 0x01,   // Р 0xD0 208
    0x3c, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x3c,   // С 0xD1 209
    0x7f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // Т 0xD2 210
    0x41, 0x41, 0x42, 0x22, 0x24, 0x24, 0x18, 0x10, 0x08, 0x04, 0x03,   // У 0xD3 211
    0x08, 0x1c, 0x2a, 0x49, 0x49, 0x49, 0x49, 0x49, 0x2a, 0x1c, 0x08,   // Ф 0xD4 212
    0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x14, 0x22, 0x22, 0x41,   // Х 0xD5 213
    0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x3f, 0x40,   // Ц 0xD6 214
    0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7e, 0x40, 0x40, 0x40, 0x40,   // Ч 0xD7 215
    0x41, 0x41, 0x41, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x7f,   // Ш 0xD8 216
    0x41, 0x41, 0x41, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x7f, 0x40,   // Щ 0xD9 217
    0x03, 0x02, 0x02, 0x02, 0x1e, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1e,   // Ъ 0xDA 218
    0x41, 0x41, 0x41, 0x41, 0x47, 0x49, 0x51, 0x51, 0x51, 0x49, 0x47,   // Ы 0xDB 219
    0x01, 0x01, 0x01, 0x01, 0x1f, 0x21, 0x41, 0x41, 0x41, 0x21, 0x1f,   // Ь 0xDC 220
    0x0f, 0x30, 0x20, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x20, 0x30, 0x0f,   // Э 0xDD 221
    0x11, 0x29, 0x45, 0x45, 0x45, 0x47, 0x45, 0x45, 0x45, 0x29, 0x11,   // Ю 0xDE 222
    0x7c, 0x42, 0x41, 0x41, 0x41, 0x42, 0x7c, 0x48, 0x44, 0x42, 0x41,   // Я 0xDF 223
    0x00, 0x00, 0x00, 0x3e, 0x41, 0x40, 0x7e, 0x41, 0x41, 0x61, 0x5e,   // а 0xE0 224
    0x1c, 0x02, 0x01, 0x1d, 0x23, 0x41, 0x41, 0x41, 0x41, 0x23, 0x1d,   // б 0xE1 225
    0x00, 0x00, 0x00, 0x1f, 0x21, 0x21, 0x1f, 0x21, 0x21, 0x21, 0x1f,   // в 0xE2 226
    0x00, 0x00, 0x00, 0x3f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   // г 0xE3 227
    0x00, 0x00, 0x00, 0x3c, 0x22, 0x22, 0x22, 0x21, 0x21, 0x21, 0x7f,   // д 0xE4 228
    0x00, 0x00, 0x00, 0x1c, 0x22, 0x41, 0x7f, 0x01, 0x01, 0x42, 0x3c,   // е 0xE5 229
    0x00, 0x00, 0x00, 0x49, 0x49, 0x2a, 0x1c, 0x2a, 0x49, 0x49, 0x49,   // ж 0xE6 230
    0x00, 0x00, 0x00, 0x3e, 0x41, 0x40, 0x3c, 0x40, 0x40, 0x41, 0x3e,   // з 0xE7 231
    0x00, 0x00, 0x00, 0x21, 0x21, 0x31, 0x29, 0x25, 0x25, 0x23, 0x21,   // и 0xE8 232
    0x00, 0x00, 0x00, 0x2d, 0x21, 0x31, 0x29, 0x25, 0x25, 0x23, 0x21,   // й 0xE9 233
    0x00, 0x00, 0x00, 0x61, 0x11, 0x09, 0x07, 0x09, 0x11, 0x21, 0x41,   // к 0xEA 234
    0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x41,   // л 0xEB 235
    0x00, 0x00, 0x00, 0x41, 0x63, 0x55, 0x49, 0x41, 0x41, 0x41, 0x41,   // м 0xEC 236
    0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x7f, 0x41, 0x41, 0x41, 0x41,   // н 0xED 237
    0x00, 0x00, 0x00, 0x1c, 0x22, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c,   // о 0xEE 238
    0x00, 0x00, 0x00, 0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,   // п 0xEF 239
    0x00, 0x00, 0x00, 0x1d, 0x23, 0x41, 0x41, 0x23, 0x1d, 0x01, 0x01,   // р 0xF0 240
    0x00, 0x00, 0x00, 0x3c, 0x42, 0x01, 0x01, 0x01, 0x01, 0x42, 0x3c,   // с 0xF1 241
    0x00, 0x00, 0x00, 0x7f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,   // т 0xF2 242
    0x00, 0x00, 0x00, 0x41, 0x41, 0x42, 0x24, 0x38, 0x10, 0x08, 0x06,   // у 0xF3 243
    0x00, 0x00, 0x00, 0x08, 0x1c, 0x2a, 0x49, 0x49, 0x2a, 0x1c, 0x08,   // ф 0xF4 244
    0x00, 0x00, 0x00, 0x41, 0x22, 0x14, 0x08, 0x08, 0x14, 0x22, 0x41,   // х 0xF5 245
    0x00, 0x00, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x3e, 0x40,   // ц 0xF6 246
    0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x41, 0x7e, 0x40, 0x40, 0x40,   // ч 0xF7 247
    0x00, 0x00, 0x00, 0x41, 0x41, 0x49, 0x49, 0x49, 0x49, 0x49, 0x7f,   // ш 0xF8 248
    0x00, 0x00, 0x00, 0x41, 0x41, 0x49, 0x49, 0x49, 0x49, 0x7f, 0x40,   // щ 0xF9 249
    0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x3e, 0x42, 0x42, 0x42, 0x3e,   // ъ 0xFA 250
    0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x4f, 0x51, 0x51, 0x51, 0x4f,   // ы 0xFB 251
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x3f, 0x41, 0x41, 0x41, 0x3f,   // ь 0xFC 252
    0x00, 0x00, 0x00, 0x1e, 0x21, 0x40, 0x7c, 0x40, 0x40, 0x21, 0x1e,   // э 0xFD 253
    0x00, 0x00, 0x00, 0x11, 0x29, 0x45, 0x47, 0x47, 0x45, 0x29, 0x11,   // ю 0xFE 254
    0x00, 0x00, 0x00, 0x7e, 0x41, 0x41, 0x41, 0x7e, 0x48, 0x44, 0x42    // я 0xFF 255
};
