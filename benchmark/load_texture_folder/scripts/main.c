#include "grngame/core/app.h"

/*
 * Environment: Desktop, 8 Threads
 * Palette Size: 32 colors
 *
 * | Test | Img Count | Unique Pixels | Grid Size | Time Before | Time After | Gain  |
 * |------|-----------|---------------|-----------|-------------|------------|-------|
 * | 1    | 100       | 128           | 100x100   | 0.979640s   | 0.224287s  | ~4.3x |
 * | 2    | 10        | 128           | 1000x1000 | 0.355364s   | 0.158079s  | ~2.2x |
 * | 3    | 100       | 1024          | 100x100   | 1.287326s   | 0.337792s  | ~3.8x |
 * | 4    | 10        | 1024          | 1000x1000 | 0.357195s   | 0.115759s  | ~3.1x |
 */

int32 main()
{
    EngineStart();
    return 0;
}
