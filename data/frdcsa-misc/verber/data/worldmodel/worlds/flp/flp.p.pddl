(define
 (problem flp1)
 (:domain flp)
 (:objects driving walking - modeOfTransportation bluetoothKeyboard tissues - object person1 person2 - person auroraIllinois chicagoIllinois - physicalLocation townhome - residence bluetoothKeyboard - tool person1sCar person2sHypotheticalCar - vehicle)
 (:init
  (autonomous person1)
  (autonomous person2)
  (location bluetoothKeyboard townhome)
  (location person1 chicagoIllinois)
  (location person1sCar chicagoIllinois)
  (location person2 townhome)
  (location person2sHypotheticalCar auroraIllinois)
  (mobile bluetoothKeyboard)
  (mobile person1sCar)
  (mobile person2sHypotheticalCar)
  (travel-path driving auroraIllinois townhome)
  (travel-path driving chicagoIllinois auroraIllinois)
  (= (travel-distance driving auroraIllinois townhome) 5)
  (= (travel-distance driving chicagoIllinois auroraIllinois) 45)
  (= (travel-duration driving auroraIllinois townhome) 0.15)
  (= (travel-duration driving chicagoIllinois auroraIllinois) 1))
 (:goal
  (and
   (directly-holding person1 bluetoothKeyboard)))
 (:metric minimize
  (total-time)))