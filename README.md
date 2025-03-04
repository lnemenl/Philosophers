# Philosophers
<p align="center">
  <img src="https://raw.githubusercontent.com/ayogun/42-project-badges/refs/heads/main/covers/cover-philosophers.png" alt="Minitalk Cover">
</p>

---

<p align="center">
  <img src="https://raw.githubusercontent.com/ayogun/42-project-badges/refs/heads/main/badges/philosopherse.png" alt="Minitalk Badge">
  <img src="https://img.shields.io/badge/Score-100%2F100-brightgreen" alt="Score Badge">
</p>

*I never thought philosophy would be so deadly*

Welcome to **Philosophers**, a multithreaded journey into the classic dining philosophers problem. This project simulates hungry philosophers sitting around a table, armed with forks and existential questions—all while battling the ultimate enemy: starvation. Using POSIX threads and mutexes, I've created a delicate dance of resource allocation that keeps our deep thinkers alive... or not.

---

## Features
- Precise simulation of the dining philosophers problem with configurable parameters
- Thread-based implementation where each philosopher lives in their own execution thread
- Mutex-protected forks ensuring no philosopher can steal another's utensil mid-bite
- Millisecond-accurate timing for eating, sleeping, thinking, and (unfortunately) dying
- Death monitoring system that catches philosophers who haven't eaten in time
- Optional meal counter to end the simulation when everyone's had their fill
- Clean, race-condition-free code

---

## Installation
1. Clone the repo:
   ```bash
   git clone https://github.com:lnemenl/Philosophers.git
   cd philo/philo
   ```
2. Compile it:
   ```bash
   make
   ```

---

## Usage
Feed your philosophers with these command-line arguments:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Parameters
- `number_of_philosophers`: How many hungry thinkers to seat at the table (also determines fork count)
- `time_to_die`: Milliseconds a philosopher can survive without eating before meeting their philosophical end
- `time_to_eat`: Milliseconds it takes to consume a meal (requires two forks)
- `time_to_sleep`: Milliseconds spent in philosophical slumber
- `number_of_times_each_philosopher_must_eat` (optional): When all philosophers have eaten this many times, they collectively decide they've had enough and the simulation ends

### Examples
```bash
# 5 hungry philosophers
./philo 5 800 200 200

# 4 philosophers who each must eat at least 5 times before the simulation ends
./philo 4 410 200 200 5

# The lone philosopher problem (spoiler: it doesn't end well)
./philo 1 800 200 200
```

---

## The Dining Dilemma
The dining philosophers problem illustrates challenges in resource allocation and deadlock prevention. Our philosophers cycle through three states:

1. **Thinking**: Contemplating the universe and waiting for forks
2. **Eating**: Consuming spaghetti with two forks (left and right)
3. **Sleeping**: Digesting both food and thoughts

The challenge? Prevent deadlocks (everyone holding one fork, waiting forever for another) and starvation (philosophers unable to eat before their time_to_die expires).

---

## Building
```bash
# Compile the program
make

# Remove object files
make clean

# Remove object files and executable
make fclean

# Rebuild everything from scratch
make re
```

---

## Implementation Details
This project showcases several key concurrent programming concepts:

- **Threads**: Each philosopher runs as an independent thread
- **Mutexes**: Forks are protected by mutexes to prevent simultaneous access
- **Race Condition Prevention**: Careful synchronization of shared resources
- **Deadlock Avoidance**: Strategic resource allocation to prevent circular waiting
- **Timing Management**: Precise tracking of eating, sleeping, and potential death times

The code carefully balances performance with safety, ensuring philosophers can eat, think, and sleep without unexpected crashes or data races.

---

## Output Format
The simulation provides real-time updates on philosopher activities:

```
timestamp_in_ms X has taken a fork  # Philosopher grabbed a utensil
timestamp_in_ms X is eating        # Philosopher is enjoying spaghetti
timestamp_in_ms X is sleeping      # Philosopher is taking a nap
timestamp_in_ms X is thinking      # Philosopher is pondering life's questions
timestamp_in_ms X died            # Philosopher has met their unfortunate end
```

Where `timestamp_in_ms` shows milliseconds since simulation start and `X` is the philosopher's number (starting from 1).

---

## Testing Scenarios

### The Basics
```bash
# Should run indefinitely (no deaths)
./philo 5 800 200 200

# All philosophers should eat at least 7 times and exit
./philo 5 800 200 200 7
```

### Edge Cases
```bash
# One philosopher with one fork = certain death
./philo 1 800 200 200

# Death should occur quickly
./philo 4 310 200 100

# Philosophers should eat exactly once and exit
./philo 4 410 200 200 1
```

---

## License
Use it, mod it, share it—just keep the copyright notice intact.
Copyright © 2024 Rus.

---

May your philosophers never starve!
