orientationOfPoints :: (Int, Int) -> (Int, Int) -> (Int, Int) -> Int 
orientationOfPoints (aX, aY) (bX, bY) (cX, cY)
    | abGradient > bcGradient = 1
    | abGradient < bcGradient = -1
    | otherwise = 0
    where abGradient = (bY - aY) * (cX - bX)
          bcGradient = (cY - bY) * (bX - aX)