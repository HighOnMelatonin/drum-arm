## https://mido.readthedocs.io/en/stable/messages/index.html#the-time-attribute
## https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html#BM1_3
import mido

def translate(file, outfile):
    ## Translate the midi file into an array of notes and times
    mid = mido.MidiFile(file)

    ## Define the notes for the center and rim of the drum
    centerNote = 40
    rimNote = 60

    ## Open the output file
    output = open(outfile, 'w')
    ## State of the arm (turned or not), not turned means arm is in start state
    turn = 0
    
    for i, track in enumerate(mid.tracks):
        print('Track {}: {}'.format(i, track.name))
        for msg in track:
            ## Block below assumes note 40 hits center and 60 hits rim
            if len(msg.bytes()) == 3:
                ## If the note is 40 and not turned, then don't turn
                if msg.bytes()[1] == centerNote and not turn:
                    turn = 0

                ## If the note is 40 and turned, then turn (return to default)
                elif msg.bytes()[1] == centerNote and turn:
                    turn = 1

                ## If the note is 60 and not turned, then turn
                elif msg.bytes()[1] == rimNote and not turn:
                    turn = 1
                
                ## If the note is 60 and turned, then don't turn
                else:
                    turn = 0

                ## Output to file
                output.write(f"{turn},{msg.time}\n")

    ## Close the output file
    print('Output written to', outfile)
    output.close()

## Test the function
if __name__ == '__main__':
    translate('raw_midi/test.mid', 'translated.csv')
