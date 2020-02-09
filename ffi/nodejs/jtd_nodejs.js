var Jtalk = require('./jtalk.js');
//// instantiation 1, default
var tts = new Jtalk();
/// instantiation 2, voice string
//var tts = new Jtalk('tohoku-f01-neutral');
//// instantiation 3, hashtable
//var tts = new Jtalk({ Voice:'mei_normal', Alpha:0.54 });

console.log("available voices:");
tts.Voices.forEach(v=>console.log(v.path));
tts.Voice = tts.Voices[Math.floor(Math.random() * tts.Voices.length)];
console.log(`current voice: ${tts.Voice.name}`);
console.log(`dic: ${tts.Dic}`);

console.log(`s  = ${tts.S}`);
console.log(`p  = ${tts.P}`);
console.log(`a  = ${tts.A}`);
console.log(`b  = ${tts.B}`);
console.log(`r  = ${tts.R}`);
console.log(`fm = ${tts.Fm}`);
console.log(`u  = ${tts.U}`);
console.log(`jm = ${tts.Jm}`);
console.log(`jf = ${tts.Jf}`);
console.log(`g  = ${tts.G}`);

tts.speakAsync('聞こえますか？');
tts.waitUntilDone();
//tts.speakToFile('録音テスト', 'test.wav');
