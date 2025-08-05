package components

import (
	"fmt"
	"github.com/charmbracelet/lipgloss"
	"strings"
)

type ProcessesBox struct {
	width, height int
	tree          []*ProcessBox
	flatView      []*ProcessBox
	selected      int
}

func (p *ProcessesBox) SetSize(w, h int) {
	p.height = h
	p.width = w
	for _, proc := range p.tree {
		proc.SetSize(p.width, 1)
	}
}

func NewProcessesBox() *ProcessesBox {
	var processes ProcessesBox
	processes = ProcessesBox{}
	processes.tree = randomizeTree()
	processes.flatView = flatten(processes.tree)

	return &processes
}

func (p *ProcessesBox) View() string {

	var usageStr strings.Builder
	usageStr.WriteString("Process Box\n")
	//usageStr.WriteString("Pid \t Program Name\t Command\t Threads\t Owner\t CpuUsage\t MemUsage\n")

	usageStr.WriteString(RenderHeader() + "\n")
	for _, proc := range p.flatView {
		usageStr.WriteString(proc.View() + "\n")
	}

	boxStyle := lipgloss.NewStyle().Width(p.width).
		Height(p.height).
		Border(lipgloss.NormalBorder()).
		Align(lipgloss.Center).
		Padding(0, 0)

	return boxStyle.Render(usageStr.String())
}

// This is the source of data, the tree field in this struct is where all the data is
// and then I use flatView to be the flattened Data responsible for rendering and all
func randomizeTree() []*ProcessBox {
	//var out []*ProcessBox
	out := []*ProcessBox{
		&ProcessBox{
			Pid:         1,
			Expanded:    true,
			ProgramName: "Zen Browser",
			Command:     "/home/zen-broswer-bin",
			MemUsage:    201.44,
			CpuUsage:    2.11,
			Owner:       "Zephyr",
			Children:    []*ProcessBox{&ProcessBox{Pid: 2, Expanded: false, ProgramName: "http-server", Command: "/home/http/server", Owner: "TempUser", MemUsage: 101.22, CpuUsage: 0.44}, &ProcessBox{Pid: 3, Expanded: true, Owner: "root", ProgramName: "frontend", Command: "/zen-browser-exe", CpuUsage: 122.222, MemUsage: 1.3}},
		},
	}
	fmt.Print(out[0].Pid)

	out2 := []*ProcessBox{
		&ProcessBox{
			Pid:         10,
			Expanded:    true,
			ProgramName: "Nova Editor",
			Command:     "/usr/bin/nova",
			MemUsage:    150.25,
			CpuUsage:    3.78,
			Owner:       "Alice",
			Children: []*ProcessBox{
				&ProcessBox{Pid: 11, Expanded: false, ProgramName: "syntax-parser", Command: "/usr/bin/syntax-parser", Owner: "Alice", MemUsage: 20.1, CpuUsage: 0.85},
				&ProcessBox{Pid: 12, Expanded: true, ProgramName: "file-watcher", Command: "/usr/bin/file-watch", Owner: "system", MemUsage: 15.4, CpuUsage: 0.32},
			},
		},
		&ProcessBox{
			Pid:         20,
			Expanded:    false,
			ProgramName: "StreamX",
			Command:     "/opt/streamx/run",
			MemUsage:    305.88,
			CpuUsage:    4.95,
			Owner:       "Bob",
			Children:    nil,
		},
		&ProcessBox{
			Pid:         30,
			Expanded:    true,
			ProgramName: "SecureShell",
			Command:     "/usr/sbin/sshd",
			MemUsage:    50.0,
			CpuUsage:    0.90,
			Owner:       "root",
			Children: []*ProcessBox{
				&ProcessBox{Pid: 31, Expanded: false, ProgramName: "user-session", Command: "/usr/sbin/sshd-session", Owner: "admin", MemUsage: 25.5, CpuUsage: 0.65},
			},
		},
		&ProcessBox{
			Pid:         40,
			Expanded:    false,
			ProgramName: "PhotoSnap",
			Command:     "/home/photosnap/photo",
			MemUsage:    180.12,
			CpuUsage:    1.25,
			Owner:       "Chloe",
			Children:    nil,
		},
		&ProcessBox{
			Pid:         50,
			Expanded:    true,
			ProgramName: "DataMiner",
			Command:     "/opt/dataminer/start",
			MemUsage:    512.7,
			CpuUsage:    9.45,
			Owner:       "data_eng",
			Children: []*ProcessBox{
				&ProcessBox{Pid: 51, Expanded: true, ProgramName: "miner-core", Command: "/opt/dataminer/core", Owner: "data_eng", MemUsage: 256.3, CpuUsage: 5.0},
			},
		},
		&ProcessBox{
			Pid:         60,
			Expanded:    false,
			ProgramName: "MailDaemon",
			Command:     "/usr/local/bin/maild",
			MemUsage:    75.0,
			CpuUsage:    1.1,
			Owner:       "postfix",
			Children:    nil,
		},
		&ProcessBox{
			Pid:         70,
			Expanded:    true,
			ProgramName: "RenderEngine",
			Command:     "/app/render/start.sh",
			MemUsage:    980.5,
			CpuUsage:    20.1,
			Owner:       "gfxuser",
			Children: []*ProcessBox{
				&ProcessBox{Pid: 71, Expanded: false, ProgramName: "gpu-handler", Command: "/app/render/gpu", Owner: "gfxuser", MemUsage: 300.3, CpuUsage: 10.2},
				&ProcessBox{Pid: 72, Expanded: false, ProgramName: "texture-loader", Command: "/app/render/texture", Owner: "gfxuser", MemUsage: 180.0, CpuUsage: 3.5},
			},
		},
		&ProcessBox{
			Pid:         81,
			Expanded:    true,
			ProgramName: "PhysicsSim",
			Command:     "/app/physics/simulate.sh",
			MemUsage:    745.3,
			CpuUsage:    25.7,
			Owner:       "simuser",
			Children: []*ProcessBox{
				&ProcessBox{Pid: 82, Expanded: false, ProgramName: "collision-core", Command: "/app/physics/collision", Owner: "simuser", MemUsage: 210.4, CpuUsage: 8.9},
				&ProcessBox{Pid: 83, Expanded: false, ProgramName: "gravity-handler", Command: "/app/physics/gravity", Owner: "simuser", MemUsage: 160.7, CpuUsage: 4.2},
			},
		},

		&ProcessBox{
			Pid:         90,
			Expanded:    false,
			ProgramName: "AIEngine",
			Command:     "/app/ai/engine.sh",
			MemUsage:    1120.0,
			CpuUsage:    33.3,
			Owner:       "aiuser",
			Children: []*ProcessBox{
				&ProcessBox{Pid: 91, Expanded: false, ProgramName: "neural-core", Command: "/app/ai/neural", Owner: "aiuser", MemUsage: 450.1, CpuUsage: 11.5},
			},
		},
	}
	return out2
}
