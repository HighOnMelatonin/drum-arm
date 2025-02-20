## https://mido.readthedocs.io/en/stable/messages/index.html#the-time-attribute
## https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html#BM1_3
import mido

def translate(file, outfile):
    ## Translate the midi file into an array of notes and times
    mid = mido.MidiFile(file)

    output = open(outfile, 'w')
    ## Open the output file, output file to be used by cpp program
    turn = 0
    
    for i, track in enumerate(mid.tracks):
        print('Track {}: {}'.format(i, track.name))
        for msg in track:
            if len(msg.bytes()) == 3:
                ## If the note is 40 and not turned, then don't turn
                if msg.bytes()[1] == 40 and not turn:
                    turn = 0

                ## If the note is 40 and turned, then turn (return to default)
                elif msg.bytes()[1] == 40 and turn:
                    turn = 1

                ## If the note is 60 and not turned, then turn
                elif msg.bytes()[1] == 60 and not turn:
                    turn = 1
                
                ## If the note is 60 and turned, then don't turn
                else:
                    turn = 0

                ## Output to file
                output.write(f"{turn},{msg.time}\n")

    output.close()

translate('raw_midi/test.mid', 'translated.csv')
