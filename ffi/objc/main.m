#import "jtalkobjc.h"

int main() {
    @autoreleasepool {
        @try {
            //JTalk *tts = [[JTalk alloc]
            // initWithVoiceName:@"tohoku-f01-neutral"
            // withDicPath:@"/usr/local/OpenJTalk/dic_utf_8"
            // withVoiceDirPath:@"/usr/local/OpenJTalk/voice"];
            JTalk *tts = [[JTalk alloc] init];

            printf("available voices:\n");
            for (id v in tts.voices) {
                printf("%s\n", [[v getPath] UTF8String]);
            }
            tts.voice = tts.voices[arc4random_uniform([tts.voices count])];
            printf("current voice: %s\n", [tts.voice.name UTF8String]);
            printf("dic: %s\n", [tts.dic UTF8String]);

            printf("s  = %u\n", tts.s);
            printf("p  = %u\n", tts.p);
            printf("a  = %f\n", tts.a);
            printf("b  = %f\n", tts.b);
            printf("r  = %f\n", tts.r);
            printf("fm = %f\n", tts.fm);
            printf("u  = %f\n", tts.u);
            printf("jm = %f\n", tts.jm);
            printf("jf = %f\n", tts.jf);
            printf("g  = %f\n", tts.g);

            [tts speakAsync:@"聞こえますか?"];
            [tts waitUntilDone];
        }
        @catch (NSException *e) {
            NSLog(@"%@", e);
        }
    }
    return 0;
}