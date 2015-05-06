class Solution {
public:
    vector<int> twoSum(vector<int> &numbers, int target) {
        vector<int> result;
        int rem = 0;
        unordered_map<int, int> mapNum;

        for (size_t i = 0; i < numbers.size() ; i++)
        {
            rem = target - numbers[i];
            auto iter = mapNum.find(rem);
            if (iter!=mapNum.end())
            {
                result.push_back(iter->second + 1);
                result.push_back(i + 1);
                break;
            }
            mapNum[numbers[i]] = i;
        }
        return result;
    }
};
