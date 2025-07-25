### Collection-And-Parsing-Engine 
#### The module, that acts as an orchestrator layer, responsible for calling backend C functions and receiving the flattened data

#### Responsibilities can be:
-- EITHER call at fixed intervals, and collect the data, whilst also keeping the availabolity of calling these backend functions on demand
-- OR convert itself to daemon and regularly dump the data into some kind of IPC or Shared Memory for data to be available for other services to read