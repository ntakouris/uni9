stored_feat_l = load('sift/ransac/left.mat');
stored_feat_r = load('sift/ransac/right.mat');

feat_l = stored_feat_l.feature; % l got the most features
feat_r = stored_feat_r.feature;

feat_count = min(size(feat_l, 2), size(feat_r, 2));

feat_matched = zeros(feat_count); % feat_matched(right_idx) = left_idx

fprintf("feat_count: %d", feat_count);

for i = 1:size(feat_r,2)
    if ~any(feat_matched(:) == 0)
        break;
    end
    
    fprintf("Iteration %d\n", i);
    
    feat = feat_l(:, i);
    
    min_dist = 99999;
    min_idx = -1;
    
    % find a non-matched one with minimum eucl distance
    for j = 1:feat_count % from the right ones
        if feat_matched(j) == 0
            feat_other = feat_r(:, j);
            dist = norm(feat-feat_other, 2);
            
            if dist < min_dist
                min_dist = dist;
                min_idx = j;
            end
        end
    end
        
    if min_dist > 0 && min_idx >= 1
        feat_matched(min_idx) = i;
        fprintf("right: %d <--> %d :left | dist: %d\n", min_idx, i, min_dist);
    end
end