# Homework

## Model the following, preferably in C++:

A local medical facility wants a model of arriving and processed patients. They can provide information about the average number of patients arriving each hour and the mean rate at which patients are processed. Model this situation in order to simulate patients arriving, processing patients, and keeping track of the number of patients waiting to be seen. The average number of patients arriving each hour, the mean rate of processing (patients/minute), and the total duration of the simulation (in minutes) should be parameters to the model. Your simulation should output relevant information for the next step of analysis (see below). Consider using [Visual Studio 2017 Community Edition](https://www.visualstudio.com/downloads/) (free) so you can provide a complete, buildable solution.

## Analyze the arrival and departure data, preferably in Python:

Process the output from your C++ simulation to analyze the patient arrival and departure information and state of the waiting room occupancy. The time scale of the analysis should be configurable, e.g. analyze in 5-minute spans, 15-minute spans, or 60 minute spans. The facility sees patients in 4-hour chunks (e.g. 8am – noon and 1:00pm – 5:00pm). Using your model, determine the number of chairs required in the waiting room if the facility sees, on average, 20 patients arriving per hour and processes a patient every 3 minutes, on average, so that the waiting room fills up no more than once every two weeks (20 4-hour chunks). (Visual Studio also supports Python projects.)

## Model the waiting room, preferably in C++:

Due to concerns about communicable diseases, arriving patients choose to seat themselves as far as possible from any other waiting patients. Using the number of chairs chosen above and the output from a run of your simulation, display the configuration of empty/occupied chairs at each arrival and processing step. This can be a text representation, e.g. X---X----X--------X for chairs 0, 4, 9, and 18 are occupied while the remainder are empty.

## Specification:

Customers often under-specify their requirements. Feel free to ask clarifying questions for this assignment.
