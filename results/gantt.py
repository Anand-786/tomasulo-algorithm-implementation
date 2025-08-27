import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as mticker

def create_gantt_chart(filename="instruction_status.txt"):
    instructions = []
    stages_data = []

    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
            for line in lines[2:]:
                parts = line.split('|')
                if len(parts) < 6:
                    continue

                instruction = parts[0].strip()
                issue = parts[1].strip()
                exec_start = parts[2].strip()
                read_mem = parts[3].strip()
                write_cdb = parts[4].strip()
                commit = parts[5].strip()

                instructions.append(instruction)
                stages_data.append({
                    'Issue': issue,
                    'ExecStart': exec_start,
                    'ReadMem': read_mem,
                    'WriteCDB': write_cdb,
                    'Commit': commit
                })
    except FileNotFoundError:
        print(f"Error: The file '{filename}' was not found. Please ensure the file is in the correct directory.")
        return

    colors = {
        'Issue': '#CBEEF3', 
        'ExecStart': '#3275AC', 
        'ReadMem': '#F26A8D', 
        'WriteCDB': '#C91D34', 
        'Commit': '#424242'
    }
    stage_names = list(colors.keys())

    fig, ax = plt.subplots(figsize=(13, 4.5))

    instructions.reverse()
    stages_data.reverse()

    y_labels = instructions
    y_pos = np.arange(len(y_labels))
    bar_height = 1.0

    max_cycle = 0
    for i, data in enumerate(stages_data):
        events = []
        for stage in stage_names:
            if data[stage] != '-':
                cycle = int(data[stage])
                events.append((cycle, stage))
                if cycle > max_cycle:
                    max_cycle = cycle

        events.sort()

        for j in range(len(events) - 1):
            start_cycle = events[j][0]
            end_cycle = events[j+1][0]
            stage_name = events[j][1]
            duration = end_cycle - start_cycle
            ax.barh(y_pos[i], duration, left=start_cycle, height=bar_height,
                    align='edge', color=colors[stage_name], edgecolor='black')

        if events:
            last_event_cycle, last_event_stage = events[-1]
            ax.barh(y_pos[i], 1, left=last_event_cycle, height=bar_height,
                    align='edge', color=colors[last_event_stage], edgecolor='black')

    ax.set_yticks(y_pos + bar_height / 2)
    ax.set_yticklabels(y_labels, fontsize=11)
    ax.invert_yaxis()

    ax.set_ylim(len(y_labels), 0)
    ax.set_xlabel('Clock Cycles', fontsize=12)
    ax.set_title('Instruction Pipeline Gantt Chart', fontsize=18, fontweight='bold', pad=20)

    ax.set_xticks(np.arange(0, max_cycle + 2, 1))
    ax.set_xticklabels(np.arange(0, max_cycle + 2, 1), fontsize=11)
    
    ax.xaxis.grid(True, linestyle='--', which='major', color='grey', alpha=.5)
    ax.set_yticks(np.arange(len(y_labels) + 1), minor=True)
    ax.yaxis.grid(True, which='minor', linestyle='--', color='grey', alpha=.5)
    ax.tick_params(axis='y', which='major', length=0)

    legend_patches = [plt.Rectangle((0,0),1,1, color=colors[stage]) for stage in stage_names]
    ax.legend(legend_patches, stage_names, bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0.)

    plt.tight_layout(rect=[0, 0, 0.92, 1])
    
    plt.show()


if __name__ == '__main__':
    create_gantt_chart("instruction_status.txt")
