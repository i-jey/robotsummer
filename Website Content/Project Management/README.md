PM Stuff, Team Collaboration etc. 
=== 

## Titles
Team #0

Robot: PL2 W7K (Pronounced "Please Work")

## The Challenge: Ewok Rescue
The 2018 ENPH253 challenge revolved around rescuing 4 plush ewoks and Chewbacca from the imperial fortress. Obstacles included two gaps/valleys in the course, an infrared-triggered spinning platform, two (optional) ziplines, and a pathless bridge separating Chewbacca from the rest of the course.

*The full rules of the competition can be found [here](https://docs.google.com/document/d/18rYIVlMZeD559deR7-5YjfQzZz4BeeS6pr8FI5Qlm8Q/edit)*

## The Strategy: Strategy and PL2 W7K
Our approach to completing the course was to have a separate mechanism for each obstacle. We designed a forklift-style mechanism to lay a bridge over the gaps. The infrared-triggered spinning platform was avoided using an active-filtering IR detection circuit. The bridge mechanism allowed us to avoid requiring the first zipline, while the second was used to transport our ewok-filled basket back to the "safe zone". The pathless bridge would be traversed using edge detection-and-follow algorithm.

PL2 was designed to be **compact**, **fast**, and **nimble**. Ewok-grabbing claws were implented in a way that allowed the robot to grab the ewoks without ever having to stop moving. Ultimatly, this was critical in our success. Since our robot could complete the course so quickly in the alotted 2 minutes we could run twice, the only robot to do so, negating any preformace deficiencies due to random unpredicable errors

## The Result: How we did
PL2 was able to consistently rescue 3 of the 5 plushies, obtaining a total of 9 points (out of 17 - Chewbacca was worth extra). Due to time constraints, we were not able to implement the second half of the course, meaning that we did not get to Chewbacca. Rather, we focused on speed and consistency, bearing us 5th place of 16 in the final competition.

## The Takeaway: What we learned
The team picked up many "dos and don'ts" along the way, as we strongly noticed what worked and what did not work. Perhaps our biggest takeaway was to plan "the small things" thoroughly to avoid spending time on them later. Examples:

- Avoid using header pins - they do not make reliable connections in wired applications

- Just commit to wire placement - Many times we were using connections that would disconnect consitantly, creating a very unreliable system. Having never designed wire managment before we were worried about changing are minds with how we did something, thus having to restart. We ended up changing how we managed our wired three times (complete reboot with solder) anyway and it would have saved time if we just commited and tried sooner.

- Decoupling Capacitors - Saved our lives. At one point when the 14.5V rail was activated and our motors started the Bluepill mirco-contoller would reset. This was likely caused through an extremly fast voltage drop along the 3.3V rail powering the mirco-contoller. A 500uF capacitor fixed that up instantly through maitining the 3.3V rail high enough for the brief moment it dropped.

- Design modularly - ensure that each part/system can be easily accessed, replaced and maintained without interfering with other systems. We learned this specifically with our powertrain. It was detachable, thus, changes could be made without invading on the rest of the chassis design.

- Continuously test - test subsystems, systems and sequences not only for functionality but also for consistency alongside development to save time debugging

- Sample size - consider outliers in your analysis but do not base your judgement single occurrences (for both failures and successes)

- Task Delegation - At some points, mainly during testing, it was not necessary for every group memeber to be present. In the event of too many people working on one componant the multitude of opinions bogged down progress. This being said, it was critical in many situations that more than one person was reviewing work to ensure mistakes where not being missed for a long time.

- Late Nights - Sometimes staying up to un-Godly hours was more of a way to have fun the work efficiently..

