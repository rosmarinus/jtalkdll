#import "jtalkobjc.h"

static const int pathbuflen = 260;

@implementation HTSVoice : NSObject
@synthesize path, name;

- (id)init:(NSString *)pathStr withName:(NSString *)nameStr {
    self = [super init];
    if (self != nil) {
        path = pathStr;
        name = nameStr;
    }
    return self;
}

- (id)init:(NSString *)pathStr {
    NSString *nameStr = [[pathStr lastPathComponent] stringByDeletingPathExtension];
    return [self init:pathStr withName:nameStr];
}

- (id)initWithCPath:(char *)pathStr {
    NSString *p = [NSString stringWithCString:pathStr encoding:NSUTF8StringEncoding];
    return [self init:p];
}

- (id)initWithCPath:(char *)pathStr withCName:(char *)nameStr {
    NSString *p = [NSString stringWithCString:pathStr encoding:NSUTF8StringEncoding];
    NSString *n = [NSString stringWithCString:nameStr encoding:NSUTF8StringEncoding];
    return [self init:p withName:n];
}

@end

@implementation JTalk : NSObject
@synthesize samplingFrequency, alpha, beta, speed, additionalHalfTone, msdThreshold,
gvWeightForSpectrum, gvWeightForLogF0, volume, s,p,a,b,r,fm,u,jm,jf,g, speaking, paused, finished, voices;

- (id)initWithVoicePath:(NSString *)voicePath withDicPath:(NSString *)dicPath withVoiceDirPath:(NSString *)voiceDirPath {
    self = [super init];
    if (self != nil) {
        handle = openjtalk_initialize([voicePath UTF8String],[dicPath UTF8String],[voiceDirPath UTF8String]);
        innerVoices = [NSMutableArray array];        
    }
    [self generateVoicelist];
    return self;
}

- (id)init {
    return [self initWithVoicePath:nil withDicPath:nil withVoiceDirPath:nil];
}

- (id)initWithVoiceName:(NSString *)name {
    return [self initWithVoiceName:name withDicPath:nil withVoiceDirPath:nil];
}

- (id)initWithVoiceName:(NSString *)name withDicPath:(NSString *)dicPath {
    return [self initWithVoiceName:name withDicPath:dicPath withVoiceDirPath:nil];
}

- (id)initWithVoiceName:(NSString *)name withDicPath:(NSString *)dicPath withVoiceDirPath:(NSString *)voiceDirPath {
    return [self initWithVoicePath:name withDicPath:dicPath withVoiceDirPath:voiceDirPath];
}

- (id)initWithVoicePath:(NSString *)voicePath {
    return [self initWithVoicePath:voicePath withDicPath:nil withVoiceDirPath:nil];
}

- (id)initWithVoicePath:(NSString *)voicePath withDicPath:(NSString *)dicPath {
    return [self initWithVoicePath:voicePath withDicPath:dicPath withVoiceDirPath:nil];
}

- (void)dealloc {
    openjtalk_clear(handle);        
    [super dealloc];
}

- (NSMutableArray *)getVoices {
    return innerVoices;
}

// 音響モデルファイルのリストを作成する
- (void)generateVoicelist {
    [innerVoices removeAllObjects];
    HtsVoiceFilelist *list = openjtalk_getHTSVoiceList(handle);
    HtsVoiceFilelist *ptr = list;
    while (ptr != nil) {
        if (ptr->succ == nil) {
            break;
        }
        HTSVoice *temp = [[HTSVoice alloc] initWithCPath:ptr->path withCName:ptr->name];
        [innerVoices addObject:temp];
        ptr = ptr->succ;
    }
    openjtalk_clearHTSVoiceList(handle,list);
}

// サンプリング周波数
- (void)setSamplingFrequency:(unsigned int)value {
    if (value < 1) {
        NSString *str = [NSString stringWithFormat:@"sampling frequency の範囲は1以上の整数です:%u",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
    }
    openjtalk_setSamplingFrequency(handle, value);
}

- (unsigned int)getSamplingFrequency {
    return openjtalk_getSamplingFrequency(handle);
}

- (void)setS:(unsigned int)value {
    [self setSamplingFrequency:value];
}

- (unsigned int)getS {
    return [self getSamplingFrequency];
}

// フレームピリオド
- (void)setFperiod:(unsigned int)value {
    if (value < 1) {
        NSString *str = [NSString stringWithFormat:@"frame period の範囲は1以上の整数です:%u",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
    }
    openjtalk_setFperiod(handle, value);
}

- (unsigned int)getFperiod {
    return openjtalk_getFperiod(handle);
}

- (void)setP:(unsigned int)value {
    [self setFperiod:value];
}

- (unsigned int)getP {
    return [self getFperiod];
}

// オールパス値
- (void)setAlpha:(double)value {
    if (value < 0.0 || value > 1.0) {
        NSString *str = [NSString stringWithFormat:@"all-pass constant の範囲は0と1の間の浮動小数点数です:%f",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
        return;
    }
    openjtalk_setAlpha(handle, value);
}

- (double)getAlpha {
    return openjtalk_getAlpha(handle);
}

- (void)setA:(double)value {
    [self setAlpha:value];
}

- (double)getA {
    return [self getAlpha];
}

// ポストフィルター係数
- (void)setBeta:(double)value {
    if (value < 0.0 || value > 1.0) {
        NSString *str = [NSString stringWithFormat:@"postfiltering coefficient の範囲は0と1の間の浮動小数点数です:%f",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
    }
    openjtalk_setBeta(handle, value);
}

- (double)getBeta {
    return openjtalk_getBeta(handle);
}

- (void)setB:(double)value {
    [self setBeta:value];
}

- (double)getB {
    return [self getBeta];
}

// スピーチ速度
- (void)setSpeed:(double)value {
    if (value < 0.0) {
        NSString *str = [NSString stringWithFormat:@"speech speed rate の範囲は0以上の浮動小数点数です:%f",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
    }
    openjtalk_setSpeed(handle, value);
}

- (double)getSpeed {
    return openjtalk_getSpeed(handle);
}

- (void)setR:(double)value {
    [self setSpeed:value];
}

- (double)getR {
    return [self getSpeed];
}

// 追加ハーフトーン
- (void)setAdditionalHalfTone:(double)value {
    openjtalk_setAdditionalHalfTone(handle, value);
}

- (double)getAdditionalHalfTone {
    return openjtalk_getAdditionalHalfTone(handle);
}

- (void)setFm:(double)value {
    [self setAdditionalHalfTone:value];
}

- (double)getFm {
    return [self getAdditionalHalfTone];
}

// 有声 / 無声境界値
- (void)setMsdThreshold:(double)value {
    if (value < 0.0 || value > 1.0) {
        NSString *str = [NSString stringWithFormat:@"voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です:%f",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
    }
    openjtalk_setMsdThreshold(handle, value);
}

- (double)getMsdThreshold {
    return openjtalk_getMsdThreshold(handle);
}

- (void)setU:(double)value {
    [self setMsdThreshold:value];
}

- (double)getU {
    return [self getMsdThreshold];
}

// スペクトラム系列内変動の重み
- (void)setGvWeightForSpectrum:(double)value {
    if (value < 0.0) {
        NSString *str = [NSString stringWithFormat:@"weight of GV for spectrum の範囲は0以上の浮動小数点数です:%f",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
    }
    openjtalk_setGvWeightForSpectrum(handle, value);
}

- (double)getGvWeightForSpectrum {
    return openjtalk_getGvWeightForSpectrum(handle);
}

- (void)setJm:(double)value {
    [self setGvWeightForSpectrum:value];
}

- (double)getJm {
    return [self getGvWeightForSpectrum];
}

- (void)setGvWeightForLogF0:(double)value {
    if (value < 0.0) {
        NSString *str = [NSString stringWithFormat:@"weight of GV for LogF0 の範囲は0以上の浮動小数点数です:%f",value];
        [[NSException exceptionWithName:@"valueError" reason:str userInfo:nil] raise];
    }
    openjtalk_setGvWeightForLogF0(handle, value);
}

- (double)getGvWeightForLogF0 {
    return openjtalk_getGvWeightForLogF0(handle);
}

- (void)setJf:(double)value {
    [self setGvWeightForLogF0:value];
}

- (double)getJf {
    return [self getGvWeightForLogF0];
}

// ボリューム
- (void)setVolume:(double)value
{
    openjtalk_setVolume(handle, value);
}

- (double)getVolume {
    return openjtalk_getVolume(handle);
}

- (void)setG:(double)value {
    [self setVolume:value];
}

- (double)getG {
    return [self getVolume];
}

// 辞書フォルダ指定
- (void)setDic:(NSString *)path {
    if ([path length]==0) {
        [[NSException exceptionWithName:@"stringIsEmpty" reason:@"辞書フォルダを示す文字列が空です" userInfo:nil] raise];
    }
    BOOL isDirectory;
    if (![[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory]) {
        NSString *str = [NSString stringWithFormat:@"辞書フォルダが見つかりません:%s",[path UTF8String]];
        [[NSException exceptionWithName:@"folderNotFound" reason:str userInfo:nil] raise];
    }
    const char *ptr = [path UTF8String];
    if (openjtalk_setDic(handle, ptr) == false) {
        NSString *str = [NSString stringWithFormat:@"辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません:%s",ptr];
        [[NSException exceptionWithName:@"IncompleteRegistration" reason:str userInfo:nil] raise];
    }
}

- (NSString *)getDic {
    char buff[pathbuflen];
    if (openjtalk_getDic(handle,buff) == nil) {
        return @"";
    }
    return [NSString stringWithCString:buff encoding:NSUTF8StringEncoding];
}

// 音響モデルファイルディレクトリ指定
- (void)setVoiceDir:(NSString *)path {
    if ([path length]==0) {
        [[NSException exceptionWithName:@"stringIsEmpty" reason:@"音響モデルフォルダを示す文字列が空です" userInfo:nil] raise];
    }
    BOOL isDirectory;
    if (![[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory])  {
        NSString *str = [NSString stringWithFormat:@"音響モデルフォルダが見つかりません:%s",[path UTF8String]];
        [[NSException exceptionWithName:@"folderNotFound" reason:str userInfo:nil] raise];
    }
    if (openjtalk_setVoiceDir(handle, [path UTF8String]) == false) {
        NSString *str = [NSString stringWithFormat:@"音響モデルフォルダを設定できません:%s",[path UTF8String]];
        [[NSException exceptionWithName:@"IncompleteRegistration" reason:str userInfo:nil] raise];
    }
    [self generateVoicelist];
}

- (NSString *)getVoiceDir {
    char buff[pathbuflen];
    if (openjtalk_getVoiceDir(handle,buff) == nil) {
        return @"";
    }
    return [NSString stringWithCString:buff encoding:NSUTF8StringEncoding];
}

// 音響モデルファイル指定
//  絶対パス...直接、相対パス...実行ファイルの位置基準での相対指定、名前のみ...探索
- (void)setVoice:(HTSVoice*)voice {
    if (voice == nil) {
        [[NSException exceptionWithName:@"stringIsEmpty" reason:@"音響モデルを示す文字列が空です" userInfo:nil] raise];
    }
    if (openjtalk_setVoice(handle, [voice.path UTF8String]) == false) {
        NSString *str = [NSString stringWithFormat:@"音響モデルを設定できません:%s",[voice.path UTF8String]];
        [[NSException exceptionWithName:@"IncompleteRegistration" reason:str userInfo:nil] raise];
    }
}

- (HTSVoice*)getVoice {
    char buff[pathbuflen];
    if (openjtalk_getVoice(handle,buff) == nil) {
        return nil;
    }
    return [[HTSVoice alloc] initWithCPath:buff];
}

// 音響モデルファイルのパスを指定および取得する
// ただし、指定時は、音声名だけかのチェックはしない
- (void)setVoicePath:(NSString *)path {
    if ([path length] == 0) {
        [[NSException exceptionWithName:@"stringIsEmpty" reason:@"音響モデルを示す文字列が空です" userInfo:nil] raise];
    }
    if (openjtalk_setVoice(handle, [path UTF8String]) == false) {
        NSString *str = [NSString stringWithFormat:@"音響モデルを設定できません:%s",[path UTF8String]];
        [[NSException exceptionWithName:@"IncompleteRegistration" reason:str userInfo:nil] raise];
    }
}

- (NSString *)getVoicePath {
    char buff[pathbuflen];
    if (openjtalk_getVoice(handle,buff) == nil) {
        return @"";
    }
    return [NSString stringWithCString:buff encoding:NSUTF8StringEncoding];
}

// 音響モデルファイルの名前を指定および取得する
// ただし、指定時は、音声名だけかのチェックはしない
- (void)setVoiceName:(NSString *)name {
    if ([name length] == 0) {
        [[NSException exceptionWithName:@"stringIsEmpty" reason:@"音響モデルを示す文字列が空です" userInfo:nil] raise];
    }
    if (openjtalk_setVoice(handle, [name UTF8String]) == false) {
        NSString *str = [NSString stringWithFormat:@"音響モデルを設定できません:%s",[name UTF8String]];
        [[NSException exceptionWithName:@"IncompleteRegistration" reason:str userInfo:nil] raise];
    }
}

- (NSString *)getVoiceName {
    char buff[pathbuflen];
    if (openjtalk_getVoice(handle,buff) == nil) {
        return @"";
    }
    NSString *str = [NSString stringWithCString:buff encoding:NSUTF8StringEncoding];
    return [[str lastPathComponent] stringByDeletingPathExtension];
}

- (bool)isSpeaking {
    return openjtalk_isSpeaking(handle);
}

- (bool)isPaused {
    return openjtalk_isPaused(handle);
}

- (bool)isFinished {
    return openjtalk_isFinished(handle);
}

- (void)speakSync:(NSString *)text {
    openjtalk_speakSync(handle, [text UTF8String]);
}

- (void)speakAsync:(NSString *)text {
    openjtalk_speakAsync(handle, [text UTF8String]);
}

- (void)pause {
    openjtalk_pause(handle);
}

- (void)resume {
    openjtalk_resume(handle);
}

- (void)stop {
    openjtalk_stop(handle);
}

- (void)waitUntilDone {
    openjtalk_waitUntilDone(handle);
}

- (void)wait {
    openjtalk_waitUntilDone(handle);
}

- (void)wait:(int)duration {
    openjtalk_wait(handle,duration);
}

// 指定ファイルに音声を記録する
- (void)speak:(NSString *)text toFile:(NSString *)file {
    if ([text length] == 0) {
        [[NSException exceptionWithName:@"stringIsEmpty" reason:@"読み上げ文字列が空です。" userInfo:nil] raise];
    }
    if ([file length] == 0) {
        [[NSException exceptionWithName:@"stringIsEmpty" reason:@"ファイル名文字列が空です。" userInfo:nil] raise];
    }
    if (openjtalk_speakToFile(handle, [text UTF8String], [file UTF8String]) == false) {
        [[NSException exceptionWithName:@"recordingError" reason:@"音声ファイルの作成中にエラーが発生しました。" userInfo:nil] raise];
    }
}

@end
