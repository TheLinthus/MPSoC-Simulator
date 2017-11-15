var author = "Caio Alexandre";
var name = "First Free";
var data = "2017-11-15";
var description = "selects the first free resource capable of supporting the execution of the task to be mapped, according to resource types and tasks, without considering performance metrics. The search for the target starts with the resource in the lower left corner of the MPSoC, and it walks through the resources column by column, always in the ascending direction";

function selectCore(mpsoc,node,application) {
    for (var i = 0; i < mpsoc.Width(); i++)	{
        for (var j = mpsoc.Height() - 1; j >= 0; j--) {
            if (mpsoc.GetCore(i,j) && mpsoc.GetCore(i,j).thread.free) {
                return {x:i,y:j};
            }
        }
    }
};
