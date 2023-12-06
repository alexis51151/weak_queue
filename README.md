<div id="top"></div>



<!-- PROJECT SHIELDS -->
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<h3 align="center">Weak Michael & Scott Non-Blocking Queue</h3>

  <p align="center">
        Implementation of the Michael & Scott Non-Blocking Queue for a Weak Memory Model to compare the performance gains, and check the correctness.
    <br />
    <br />
  </p>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

We have implemented the following data structures:
* MS Queue for Strong Memory Model (x86)

We have model checked using CDSChecker:
* MS Queue for Strong Memory Model (x86) w/o freeing memory

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

* C++ 11
* CMake
* CPU with weak memory ordering (ex: POWER9, Apple Silicon, ARM...)


<!-- USAGE EXAMPLES -->
## Testing

### Strong Michael & Scott Queue
The implementation for the strong MS queue is located at `src/ms_queue_strong`.
To run the tests:

1. Go to the directory: `cd src/ms_queue_strong`
2. Build the project: `cmake .; make`
3. Run the tests: `./test_seq; ./test_par`

### Weak Michael & Scott Queue
The implementation for the weak MS queue is located at `src/ms_queue_weak`.
To run the tests:

1. Go to the directory: `cd src/ms_queue_weak`
2. Build the project: `cmake .; make`
3. Run the tests: `./test_seq; ./test_par`

## Model Checking
We have tested our strong and weak implementations of the MS queue using the Model Checker CDSChecker.


Because CDSChecker does not support atomic operations over datatype larger than 64 bits, we have simplified the algorithm:
* Queue items are 32-bit unsigned integers,
* No deallocation of memory (to prevent ABA issues when removing the Pointer structure).

### CDSChecker
To run CDSChecker over the strong and weak algorithms, follow those instructions:

#### Strong Michael & Scott Queue
1. Go to the directory: `cd CDSChecker/ms-queue/ms-queue-strong-no-free`
2. Compile the binary: `make`
3. Run CDSChecker: `./../../run.sh test_2threads  -y -m 2`

#### Weak Michael & Scott Queue
1. Go to the directory: `cd CDSChecker/ms-queue/ms-queue-weak-no-free`
2. Compile the binary: `make`
3. Run CDSChecker: `./../../run.sh test_2threads  -y -m 2`

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Alexis Le Glaunec - alexis.leglaunec@rice.edu

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[license-shield]: https://img.shields.io/github/license/alexis51151/SAT-DPLL.svg?style=for-the-badge
[license-url]: https://github.com/alexis51151/SAT-DPLL/blob/master/LICENSE.md
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/alexis-leglaunec
[product-screenshot]: images/screenshot.png
