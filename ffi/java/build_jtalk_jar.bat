javac -encoding UTF-8 -classpath jna-4.4.0.jar;jna-platform-4.4.0.jar; -Xlint:unchecked -Xdiags:verbose JTalkJna.java
if not exist mkdir mkdir jtalk
move JTalkJna*.class jtalk\
jar vcf jtalk.jar jtalk
