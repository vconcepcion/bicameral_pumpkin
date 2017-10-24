require 'rubyserial'

# Update this line with device identifier
$arduino_serial = Serial.new("/dev/cu.usbmodem135", 9600)

load_library :minim
include_package 'ddf.minim'
include_package 'ddf.minim.analysis'

attr_reader :music, :waiting, :bell, :game_started

def setup
  minim = Minim.new(self)
  @music = minim.load_file('fig_leaf_rag_x2.mp3', 1_024)
  @waiting = minim.load_file('five_card_shuffle.mp3', 1_024)
  @bell = minim.load_sample('bell.mp3')
  @game_started = false
  waiting.loop
end

def draw
  if music.position >= 4800 && !game_started
    bell.trigger
		$arduino_serial.write('S');
    @game_started = true
  end

  ch = $arduino_serial.read(1)
  bell.trigger if ch == 'O'
end

def key_pressed
  if key == 's' && !game_started
		$arduino_serial.write('P');
    waiting.pause
    music.loop
  elsif key == 'q'
    @game_started = false
    music.pause
    music.rewind
    waiting.loop
		$arduino_serial.write('Q');
  end
end
