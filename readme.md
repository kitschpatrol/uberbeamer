<!-- title -->

# überbeamer

<!-- /title -->

<!-- short-description -->

**A means of making any surface writeable. ITP thesis project, 2011.**

<!-- /short-description -->

## Overview

The überbeamer is a means of making any surface writeable. It was created by Eric Mika as an [ITP](https://itp.nyu.edu/) thesis project in the spring of 2011. It was presented at the [Art && Code 3D](https://3d.artandcode.org/) conference.

The hardware is a projector with specially-prepared autofocus servo, a Wiimote, a Kinect, and a MacBook Pro in a custom-fabricated enclosure worn on a shoulder strap.

This repository holds the [openFrameworks](https://openframeworks.cc/) app that ran it. The app implements "inside out" 6-DOF tracking and modeling of its environment, and records the points draw into that model so they can be projected back onto the surface. It also implements "autofocus" for the projector with a servo, driven by the Kinect's average depth reading.

A [video of the thesis presentation](https://vimeo.com/29528066) is on Vimeo, and photos of the build are on the [process blog](https://uberbeamer.tumblr.com/).

The [project site](https://uberbeamer.com) provides additional links and details.
