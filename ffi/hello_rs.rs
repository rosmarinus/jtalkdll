use std::ffi::CString;
use std::os::raw::c_char;
use std::os::raw::c_void;
#[link(name = "jtalk")]
extern "stdcall" {
	fn OpenJTalk_initialize() -> *const c_void;
	//fn OpenJTalk_speakSync( handle: *const c_void, Text: *const c_char);
	fn OpenJTalk_setVoice( handle: *const c_void, Text: *const c_char);
	fn OpenJTalk_speakAsync( handle: *const c_void, Text: *const c_char);
	fn OpenJTalk_waitUntilDone( handle: *const c_void);
	fn OpenJTalk_clear(handle: *const c_void);
}

fn speak(voice: &str, message: &str) {
	let v = CString::new(voice).unwrap();
	let m = CString::new(message).unwrap();
	unsafe{
		let h = OpenJTalk_initialize();
		OpenJTalk_setVoice(h, v.as_ptr());
		OpenJTalk_speakAsync(h, m.as_ptr());
		OpenJTalk_waitUntilDone(h);
		OpenJTalk_clear(h);
	}
}

fn main() {
	//speak("mei_normal","こんにちは");
	let v = CString::new("tohoku-f01-neutral").unwrap();
	let m = CString::new("hello").unwrap();
	unsafe{
		let h = OpenJTalk_initialize();
		OpenJTalk_setVoice(h, v.as_ptr());
		OpenJTalk_speakAsync(h, m.as_ptr());
		OpenJTalk_waitUntilDone(h);
		OpenJTalk_clear(h);
	}
}

