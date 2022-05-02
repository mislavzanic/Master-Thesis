factorial :: (Eq a, Num a) => a -> a
factorial n = if n == 0 then 1 else n * factorial (n-1)

sign :: (Ord a, Num a, Num b) => a -> b
sign x | x < 0 = -1
       | x > 0 = 1
       | otherwise = 0

prod :: (Num a) => [a] -> a
prod = foldr (\acc x -> acc * x) 1

reverse' :: [a] -> [a]
reverse' [] = []
reverse' (x:xs) = reverse' xs ++ [x]

double' :: (Num a) => [a] -> [a]
double' = map (\x -> 2 * x)

merge :: (Ord a) => [a] -> [a] -> [a]
merge [] l = l
merge l [] = l
merge (x:xs) (y:ys) | x < y = x : merge xs (y:ys)
                    | otherwise = y : merge (x:xs) ys

mergeSort :: (Ord a) => [a] -> [a]
mergeSort [] = []
mergeSort [x] = [x]
mergeSort x = merge (mergeSort l) (mergeSort r)
  where
    l = take (length x `div` 2) x
    r = drop (length x `div` 2) x
