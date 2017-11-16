var author = "Caio Alexandre";
var name = "First Free";
var date = "2017-11-16";
var description = "selects the first free resource capable of supporting the execution of the task to be mapped, according to resource types and tasks, without considering performance metrics. The search for the target starts with the resource in the lower left corner of the MPSoC, and it walks through the resources column by column, always in the ascending direction";

function selectCore(mpsoc,node,application) {
    for (var i = 0; i < mpsoc.width; i++)	{
        for (var j = mpsoc.height - 1; j >= 0; j--) {
            if (mpsoc.cores[i][j] && mpsoc.cores[i][j].threads[0].free) {
                return {x:i,y:j};
            }
        }
    }
};
