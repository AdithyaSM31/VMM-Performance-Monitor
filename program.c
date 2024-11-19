#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GRAPH_WIDTH 400
#define GRAPH_HEIGHT 100
#define NUM_POINTS 50
#define ANALYSIS_PERIOD 30  // Time in seconds for calculating average performance score

static GtkWidget *cpu_graph;
static GtkWidget *mem_graph;
static GtkWidget *disk_graph;
static GtkWidget *net_graph;
static GtkWidget *cpu_value_label;
static GtkWidget *mem_value_label;
static GtkWidget *disk_value_label;
static GtkWidget *net_value_label;
static GtkWidget *score_label;

double cpu_usage_data[NUM_POINTS] = {0};
double mem_usage_data[NUM_POINTS] = {0};
double disk_io_data[NUM_POINTS] = {0};
double net_latency_data[NUM_POINTS] = {0};

double cpu_sum = 0;
double mem_sum = 0;
double disk_sum = 0;
double net_sum = 0;
int sample_count = 0;
double constant_performance_score = 0;

// Placeholder functions for data retrieval
double getCpuUsage() { return (rand() % 100); }
double getMemoryUsage() { return (rand() % 100); }
double getDiskIOPerformance() { return (rand() % 100); }
double getNetworkLatency() { return (rand() % 100); }

// Update data array for scrolling effect
void update_data(double data[], double new_value) {
    memmove(&data[1], &data[0], (NUM_POINTS - 1) * sizeof(double));
    data[0] = new_value;
}

// Calculate a performance score based on metric averages
double calculate_average_performance_score(double avg_cpu, double avg_mem, double avg_disk, double avg_net) {
    double score = 100;

    // CPU usage - lower is better
    if (avg_cpu < 20) score += 10;
    else if (avg_cpu < 50) score += 5;
    else score -= 10;

    // Memory usage - lower is better
    if (avg_mem < 30) score += 10;
    else if (avg_mem < 70) score += 5;
    else score -= 10;

    // Disk I/O - higher is better
    if (avg_disk > 80) score += 10;
    else if (avg_disk > 50) score += 5;
    else score -= 10;

    // Network latency - lower is better
    if (avg_net < 50) score += 10;
    else if (avg_net < 100) score += 5;
    else score -= 10;

    // Ensure score stays within 0-100 range
    if (score > 100) score = 100;
    if (score < 0) score = 0;

    return score;
}

// Draw the graph with unique colors
gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data) {
    double *usage_data = (double *)data;

    // Set background color (dark gray)
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_rectangle(cr, 0, 0, GRAPH_WIDTH, GRAPH_HEIGHT);
    cairo_fill(cr);

    // Select color based on the widget pointer (for unique colors)
    if (widget == cpu_graph) cairo_set_source_rgb(cr, 0.0, 0.5, 1.0);  // Blue for CPU
    else if (widget == mem_graph) cairo_set_source_rgb(cr, 0.0, 0.8, 0.2);  // Green for Memory
    else if (widget == disk_graph) cairo_set_source_rgb(cr, 1.0, 0.8, 0.0);  // Yellow for Disk I/O
    else if (widget == net_graph) cairo_set_source_rgb(cr, 1.0, 0.2, 0.2);  // Red for Network Latency

    cairo_set_line_width(cr, 2);

    // Draw the line graph
    for (int i = 0; i < NUM_POINTS - 1; i++) {
        cairo_move_to(cr, i * (GRAPH_WIDTH / (NUM_POINTS - 1)), GRAPH_HEIGHT - usage_data[i]);
        cairo_line_to(cr, (i + 1) * (GRAPH_WIDTH / (NUM_POINTS - 1)), GRAPH_HEIGHT - usage_data[i + 1]);
    }
    cairo_stroke(cr);

    return FALSE;
}

// Refresh data and redraw graphs every second
gboolean refresh_graphs(gpointer user_data) {
    if (sample_count < ANALYSIS_PERIOD) {
        double cpu_value = getCpuUsage();
        double mem_value = getMemoryUsage();
        double disk_value = getDiskIOPerformance();
        double net_value = getNetworkLatency();

        // Collect cumulative data for averaging
        cpu_sum += cpu_value;
        mem_sum += mem_value;
        disk_sum += disk_value;
        net_sum += net_value;
        sample_count++;

        // Update data arrays for graphs
        update_data(cpu_usage_data, cpu_value);
        update_data(mem_usage_data, mem_value);
        update_data(disk_io_data, disk_value);
        update_data(net_latency_data, net_value);

        // Update labels with current values
        char value_text[50];
        snprintf(value_text, sizeof(value_text), "CPU: %.2f %%", cpu_value);
        gtk_label_set_text(GTK_LABEL(cpu_value_label), value_text);

        snprintf(value_text, sizeof(value_text), "Memory: %.2f %%", mem_value);
        gtk_label_set_text(GTK_LABEL(mem_value_label), value_text);

        snprintf(value_text, sizeof(value_text), "Disk I/O: %.2f MB/s", disk_value);
        gtk_label_set_text(GTK_LABEL(disk_value_label), value_text);

        snprintf(value_text, sizeof(value_text), "Network: %.2f ms", net_value);
        gtk_label_set_text(GTK_LABEL(net_value_label), value_text);

        gtk_widget_queue_draw(cpu_graph);
        gtk_widget_queue_draw(mem_graph);
        gtk_widget_queue_draw(disk_graph);
        gtk_widget_queue_draw(net_graph);
    } else if (constant_performance_score == 0) {
        // Calculate average values
        double avg_cpu = cpu_sum / sample_count;
        double avg_mem = mem_sum / sample_count;
        double avg_disk = disk_sum / sample_count;
        double avg_net = net_sum / sample_count;

        // Calculate constant performance score and display it
        constant_performance_score = calculate_average_performance_score(avg_cpu, avg_mem, avg_disk, avg_net);
        char score_text[50];
        snprintf(score_text, sizeof(score_text), "Performance Score: %.2f", constant_performance_score);
        gtk_label_set_text(GTK_LABEL(score_label), score_text);
    }

    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Real-Time Performance Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 650);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Vertical box for organizing widgets
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Add labels and graphs for each metric with value labels

    // CPU Usage
    GtkWidget *cpu_label = gtk_label_new("CPU Usage (%)");
    gtk_box_pack_start(GTK_BOX(vbox), cpu_label, FALSE, FALSE, 0);
    cpu_graph = gtk_drawing_area_new();
    gtk_widget_set_size_request(cpu_graph, GRAPH_WIDTH, GRAPH_HEIGHT);
    g_signal_connect(cpu_graph, "draw", G_CALLBACK(draw_graph), cpu_usage_data);
    gtk_box_pack_start(GTK_BOX(vbox), cpu_graph, TRUE, TRUE, 0);
    cpu_value_label = gtk_label_new("CPU: 0.00 %");
    gtk_box_pack_start(GTK_BOX(vbox), cpu_value_label, FALSE, FALSE, 0);

    // Memory Usage
    GtkWidget *mem_label = gtk_label_new("Memory Usage (%)");
    gtk_box_pack_start(GTK_BOX(vbox), mem_label, FALSE, FALSE, 0);
    mem_graph = gtk_drawing_area_new();
    gtk_widget_set_size_request(mem_graph, GRAPH_WIDTH, GRAPH_HEIGHT);
    g_signal_connect(mem_graph, "draw", G_CALLBACK(draw_graph), mem_usage_data);
    gtk_box_pack_start(GTK_BOX(vbox), mem_graph, TRUE, TRUE, 0);
    mem_value_label = gtk_label_new("Memory: 0.00 %");
    gtk_box_pack_start(GTK_BOX(vbox), mem_value_label, FALSE, FALSE, 0);

    // Disk I/O Performance
    GtkWidget *disk_label = gtk_label_new("Disk I/O Performance (MB/s)");
    gtk_box_pack_start(GTK_BOX(vbox), disk_label, FALSE, FALSE, 0);
    disk_graph = gtk_drawing_area_new();
    gtk_widget_set_size_request(disk_graph, GRAPH_WIDTH, GRAPH_HEIGHT);
    g_signal_connect(disk_graph, "draw", G_CALLBACK(draw_graph), disk_io_data);
    gtk_box_pack_start(GTK_BOX(vbox), disk_graph, TRUE, TRUE, 0);
    disk_value_label = gtk_label_new("Disk I/O: 0.00 MB/s");
    gtk_box_pack_start(GTK_BOX(vbox), disk_value_label, FALSE, FALSE, 0);

    // Network Latency
    GtkWidget *net_label = gtk_label_new("Network Latency (ms)");
    gtk_box_pack_start(GTK_BOX(vbox), net_label, FALSE, FALSE, 0);
    net_graph = gtk_drawing_area_new();
    gtk_widget_set_size_request(net_graph, GRAPH_WIDTH, GRAPH_HEIGHT);
    g_signal_connect(net_graph, "draw", G_CALLBACK(draw_graph), net_latency_data);
    gtk_box_pack_start(GTK_BOX(vbox), net_graph, TRUE, TRUE, 0);
    net_value_label = gtk_label_new("Network: 0.00 ms");
    gtk_box_pack_start(GTK_BOX(vbox), net_value_label, FALSE, FALSE, 0);

    // Performance Score (constant)
    score_label = gtk_label_new("Performance Score: Analyzing...");
    gtk_box_pack_start(GTK_BOX(vbox), score_label, FALSE, FALSE, 0);

    // Set a timer to refresh graphs every second
    g_timeout_add(1000, refresh_graphs, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
