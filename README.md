# About ofxLSL

#### Dependency
- In order to run the example, liblsl64.dll must be in the app bin folder with the compiled .exe
- Because lsl libaries are written for x64 systems, ensure that Visual Studio Platforms are set to x64

#### Accuracy
- timestamp,_TS_, is 100% accurate
- timestampLocal, _TSC_, periodicity is at worst accurate to 1.8ms
  - average: 19 microseconds
- localClock, _LC_, periodicity is rather unreliable (>300ms)
- Can be more accurate if tags are sent and recieved on same computer

####Python Tag Code
- **Marker.py**
  - begins stream that alternates printing "Hello" and "World"
  - Tags sent aproximately ever 1.5 sec
- **MarkerCSV.py**
  - same basic function as Marker.py
  - writes the timestamps of the tags to an external file, _sentInfo.csv_
- These functions can be used to test the basic functionality of ofxLSL

#### resolvestreams() arguments
- **name**
  - Name of the stream. Describes the device (or product series) that this stream makes available (for use by programs, experimenters or data analysts)
  - Cannot be empty.
- **type**
  - Content type of the stream
  - By convention LSL uses the content types defined in the XDF file format specification where applicable (https://github.com/sccn/xdf).
  - The content type is the preferred way to find streams (as opposed to searching by name).
- **channel_count** 
  - Number of channels per sample. This stays constant for the lifetime of the stream.
  - default = 1
- **nominal_srate**
  - The sampling rate (in Hz) as advertised by the data source, regular (otherwise set to IRREGULAR_RATE).
  - default = IRREGULAR_RATE
- **channel_format**
  - Format/type of each channel. If your channels have different formats, consider supplying multiple streams or use the largest type that can hold  them all (such as cf_double64). 
  - Passed as a string, without the cf_ prefix,
    - e.g., 'float32' (default cf_float32)
- **source_id**
  - Unique identifier of the device or source of the data, if available (such as the serial number).
  - Critical for system robustness since it allows recipients to recover from failure even after the serving app, device or computer crashes (just by finding a stream with the same source id on the network again).
  - Highly recommended to always try to provide whatever information can uniquely identify the data source itself.
  - default = ''
