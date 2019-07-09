"""Minimal example of how to send event triggers in PsychoPy with
LabStreamingLayer.
In this example, the words "hello" and "world" alternate on the screen, and
an event marker is sent with the appearance of each word.
TO RUN: open in PyschoPy Coder and press 'Run'. Or if you have the psychopy
Python package in your environment, run `python hello_world.py` in command line.
ID     EVENT
------------
1  --> hello
2  --> world
99 -->  test
------------
"""
from psychopy import core, visual, event
from pylsl import StreamInfo, StreamOutlet, local_clock
import pandas as pd
from pandas import DataFrame as df
from IPython.display import display as dsply

def main():
    """Alternate printing 'Hello' and 'World' and send a trigger each time."""
    # Set up LabStreamingLayer stream.
    info = StreamInfo(name='CFL', type='Tags', channel_count=1,
                      channel_format='string', source_id='')
    outlet = StreamOutlet(info)  # Broadcast the stream.

    # This is not necessary but can be useful to keep track of markers and the
    # events they correspond to.
    markers = {
        'hello': ["Hello"],
        'world': ["World"],
        'test': ["Test5"],
    }
    
    data = df(columns = ['Timestamp','Data'])
    
    # Send triggers to test communication.
    for _ in range(5):
        outlet.push_sample(markers['test'])
        core.wait(0.5)

    # Instantiate the PsychoPy window and stimuli.
    win = visual.Window([800, 600], allowGUI=False, monitor='testMonitor',
                        units='deg')
    hello = visual.TextStim(win, text="Hello")
    world = visual.TextStim(win, text="World")

    for i in range(100):
        if not i % 2:  # If i is even:
            hello.draw()
            # # Experiment with win.callOnFlip method. See Psychopy window docs.
            # win.callOnFlip(outlet.push_sample, markers['hello'])
            win.flip()
            ts = local_clock()
            data = data.append({'Timestamp':ts,'Data':'Hello'},ignore_index=True)
            outlet.push_sample(markers['hello'],timestamp=ts)
        else:
            world.draw()
            # win.callOnFlip(outlet.push_sample, markers['world'])
            win.flip()
            ts=local_clock()
            data = data.append({'Timestamp':ts,'Data':'World'},ignore_index=True)
            outlet.push_sample(markers['world'],timestamp=ts)
        if 'escape' in event.getKeys():  # Exit if user presses escape.
            break
        core.wait(1.0)  # Display text for 1.0 second.
        win.flip()
        core.wait(0.5)  # ISI of 0.5 seconds.
    #dsply(data)
    data.to_csv('c:/users/ctjen/desktop/sentInfo.csv')
    win.close()
    core.quit()
   

if __name__ == "__main__":
    main()
