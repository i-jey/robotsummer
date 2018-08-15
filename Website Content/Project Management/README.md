PM Stuff, Team Collaboration etc. 
=== 

## Titles
Team #0

Robot: PL2 W7K (Pronounced "Please Work")

## The Challenge: Ewok Rescue
The 2018 ENPH253 challenge revolved around rescuing 4 plush ewoks and Chewbacca from the imperial fortress. Obstacles included two gaps/valleys in the course, an infrared-triggered spinning platform, two (optional) ziplines, and a pathless bridge separating Chewbacca from the rest of the course.

*The full rules of the competition can be found here: https://docs.google.com/document/d/18rYIVlMZeD559deR7-5YjfQzZz4BeeS6pr8FI5Qlm8Q/edit*

## The Strategy: Strategy and PL2 W7K
Our approach to completing the course was to have a separate mechanism for each obstacle. We designed a forklift-style mechanism to lay a bridge over the gaps. The infrared-triggered spinning platform was avoided using an active-filtering IR detection circuit. The bridge mechanism allowed us to avoid requiring the first zipline, while the second was used to transport our ewok-filled basket back to the "safe zone". The pathless bridge would be traversed using edge detection-and-follow algorithm.

PL2 was designed to be compact, fast, and nimble. Ewok-grabbing claws were implented in a way that allowed the robot to grab the ewoks without ever having to stop moving.

## The Result: How we did
PL2 was able to consistently rescue 3 of the 5 plushies, obtaining a total of 9 points (out of 17 - Chewbacca was worth extra). Due to time constraints, we were not able to implement the second half of the course, meaning that we did not get to Chewbacca. Rather, we focused on speed and consistency, bearing us 5th place in the final competition.

## The Takeaway: What we learned
The team picked up many "dos and don'ts" along the way, as we strongly noticed what worked and what did not work. Perhaps our biggest takeaway was to plan "the small things" thoroughly to avoid spending time on them later. Examples:

- Avoid using header pins - they do not make reliable connections in wired applications

-
