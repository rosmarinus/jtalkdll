#import <Foundation/Foundation.h>
#import "jtalk.h"

@interface HTSVoice : NSObject
@property (nonatomic, readonly, getter=getPath) NSString* path;
@property (nonatomic, readonly, getter=getName) NSString* name;
- (id)init:(NSString*)pathStr;
- (id)initWithCPath:(char *)pathStr;
- (id)init:(NSString*)pathStr withName:(NSString *)nameStr;
- (id)initWithCPath:(char *)pathStr withCName:(char *)nameStr;
@end

@interface JTalk : NSObject
{
    OpenJTalk *handle;
    NSMutableArray *innerVoices;
}
@property (nonatomic, getter=getSamplingFrequency, setter=setSamplingFrequency:) unsigned int samplingFrequency;
@property (nonatomic, getter=getS, setter=setS:) unsigned int s;
@property (nonatomic, getter=getFperiod, setter=setFperiod:) unsigned int fperiod;
@property (nonatomic, getter=getP, setter=setP:) unsigned int p;
@property (nonatomic, getter=getAlpha, setter=setAlpha:) double alpha;
@property (nonatomic, getter=getA, setter=setA:) double a;
@property (nonatomic, getter=getBeta, setter=setBeta:) double beta;
@property (nonatomic, getter=getB, setter=setB:) double b;
@property (nonatomic, getter=getSpeed, setter=setSpeed:) double speed;
@property (nonatomic, getter=getR, setter=setR:) double r;
@property (nonatomic, getter=getAdditionalHalfTone, setter=setAdditionalHalfTone:) double additionalHalfTone;
@property (nonatomic, getter=getFm, setter=setFm:) double fm;
@property (nonatomic, getter=getMsdThreshold, setter=setMsdThreshold:) double msdThreshold;
@property (nonatomic, getter=getU, setter=setU:) double u;
@property (nonatomic, getter=getGvWeightForSpectrum, setter=setGvWeightForSpectrum:) double gvWeightForSpectrum;
@property (nonatomic, getter=getJm, setter=setJm:) double jm;
@property (nonatomic, getter=getGvWeightForLogF0, setter=setGvWeightForLogF0:) double gvWeightForLogF0;
@property (nonatomic, getter=getJf, setter=setJf:) double jf;
@property (nonatomic, getter=getVolume, setter=setVolume:) double volume;
@property (nonatomic, getter=getG, setter=setG:) double g;
@property (nonatomic, readonly, getter=isSpeaking) bool speaking;
@property (nonatomic, readonly, getter=isPaused) bool paused;
@property (nonatomic, readonly, getter=isFinished) bool finished;
@property (nonatomic, readonly, getter=getVoices) NSMutableArray* voices;

@property (nonatomic, copy, getter=getVoice, setter=setVoice:) HTSVoice *voice;
@property (nonatomic, copy, getter=getVoiceDir, setter=setVoiceDir:) NSString *voiceDir;
@property (nonatomic, copy, getter=getDic, setter=setDic:) NSString *dic;
@property (nonatomic, copy, getter=getVoicePAth, setter=setVoicePath:) NSString *voicePath;
@property (nonatomic, copy, getter=getVoiceNamr, setter=setVoiceName:) NSString *voiceName;

- (id)initWithVoicePath:(NSString *)voicePath withDicPath:(NSString *)dicPath withVoiceDirPath:(NSString *)voiceDirPath;
- (id)init;
- (id)initWithVoiceName:(NSString *)name;
- (id)initWithVoiceName:(NSString *)name withDicPath:(NSString *)dicPath;
- (id)initWithVoiceName:(NSString *)name withDicPath:(NSString *)dicPath withVoiceDirPath:(NSString *)voiceDirPath;
- (id)initWithVoicePath:(NSString *)voicePath;
- (id)initWithVoicePath:(NSString *)voicePath withDicPath:(NSString *)dicPath;

- (void)speakSync:(NSString*)text;
- (void)speakAsync:(NSString*)text;
- (void)pause;
- (void)resume;
- (void)stop;
- (void)waitUntilDone;
- (void)wait;
- (void)wait:(int)duration;
- (void)speak:(NSString*)text toFile:(NSString*)file;
@end
