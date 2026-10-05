// Decompiled by Opus. Names are provisional.
// The constructor of the 0x294-byte sound/volume object (allocated by
// 0x49ea3d); 0x4cff30 opens the devices and 0x4d0040 reads the aux volume.

class Class_004cff30 {
public:
    void InitMixerVolumes();
};

class Class_004d0040 {
public:
    int QueryAuxVolume();
};

class Class_004cee50 {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    float field_8;                     // +0x8
    float field_c;                     // +0xc
    int wave_devices;                  // +0x10
    unsigned int aux_device;           // +0x14
    int wave_volume;                   // +0x18
    int aux_volume;                    // +0x1c
    int aux_volume_set;                // +0x20
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30
    int field_34;                      // +0x34
    int array_38[32];                  // +0x38
    int array_b8[32];                  // +0xb8
    int array_138[32];                 // +0x138
    char unknown_1b8[0x1c4 - 0x1b8];
    int array_1c4[8];                  // +0x1c4
    int field_1e4;                     // +0x1e4
    char unknown_1e8[0x284 - 0x1e8];
    int field_284;                     // +0x284
    int field_288;                     // +0x288
    char unknown_28c[0x290 - 0x28c];
    int field_290;                     // +0x290

    Class_004cee50();
};

// FUNCTION: 0x4cee50
Class_004cee50::Class_004cee50()
{
    field_0 = 0;
    field_4 = 0;
    field_290 = 0;
    field_8 = 1.0f;
    field_c = 1e20f;
    field_24 = 0;
    field_28 = 0;
    for (int j = 0; j < 8; j++) {
        array_1c4[j] = 0;
    }
    ((Class_004cff30*)this)->InitMixerVolumes();
    field_2c = 8;
    field_30 = 0;
    field_34 = 1;
    for (int i = 0; i < 32; i++) {
        array_38[i] = 0;
        array_b8[i] = 0;
        array_138[i] = 0;
    }
    field_1e4 = 0;
    aux_volume_set = ((Class_004d0040*)this)->QueryAuxVolume();
    field_288 = -1;
}
