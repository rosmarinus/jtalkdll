extern "C" fn on_finishied() {println!("<読み上げ完了>");}

fn main() {
    let tts = jtalk::jtalk::TTS::new("","","");
    tts.regist_on_finished_callback(Some(on_finishied));
    println!("available voices:");
    tts.get_voices().iter().for_each(|i|println!("{}",i.path));
    println!("current voice: {}",tts.get_voice().name);
    println!("dic: {}",tts.get_dic());
    println!("s  = {}",tts.get_s());
    println!("p  = {}",tts.get_p());
    println!("a  = {}",tts.get_a());
    println!("b  = {}",tts.get_b());
    println!("r  = {}",tts.get_r());
    println!("fm = {}",tts.get_fm());
    println!("u  = {}",tts.get_u());
    println!("jm = {}",tts.get_jm());
    println!("jf = {}",tts.get_jf());
    println!("g  = {}",tts.get_g());
    tts.speak_async("聞こえてますか？");
    tts.wait_until_done();
}
