## https://mido.readthedocs.io/en/stable/messages/index.html#the-time-attribute
## https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html#BM1_3
import mido

def translate(file):
    ## Translate the midi file into an array of notes and times
    mid = mido.MidiFile(file)
    ## Open the output file, output file to be used by cpp program
    output = open('output.txt', 'w')
    for i, track in enumerate(mid.tracks):
        print('Track {}: {}'.format(i, track.name))
        for msg in track:
            if len(msg.bytes()) == 3:
                print(f"Note: {msg.bytes()[1]} | Time: {msg.time}")

translate('raw_midi/test.mid')
