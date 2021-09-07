#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/filters/statistical_outlier_removal.h>
 #include <pcl/visualization/cloud_viewer.h>
 
using namespace std;

int main(){
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile<pcl::PointXYZ>("out.pcd", *cloud);
    pcl::PCDWriter writer;
    //cout << cloud->width * cloud->height << endl;
    
    //降采样
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered1 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered2 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered3 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud (cloud);
    pass.setFilterFieldName ("z");
    pass.setFilterLimits (0.0, 1.0);
    //pass.setFilterLimitsNegative (true);
    pass.filter (*cloud_filtered1);
    
    //网格质心
    pcl::VoxelGrid<pcl::PointXYZ> vg;
    vg.setInputCloud (cloud);
    vg.setLeafSize (0.01f, 0.01f, 0.01f);
    vg.filter (*cloud_filtered1);
    
    //除去稀疏异常点
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
    sor.setInputCloud (cloud_filtered1);
    sor.setMeanK (50);
    sor.setStddevMulThresh (1.0);
    sor.filter (*cloud_filtered2);
    
    //除去稀疏异常值
    pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;
    outrem.setInputCloud(cloud_filtered2);
    outrem.setRadiusSearch(0.8);
    outrem.setMinNeighborsInRadius (2);
    outrem.setKeepOrganized(true);
    outrem.filter (*cloud_filtered3);
    
    //平面模型切割
    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
    pcl::SACSegmentation<pcl::PointXYZ> seg;
    seg.setOptimizeCoefficients (true);
    seg.setModelType (pcl::SACMODEL_PLANE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setDistanceThreshold (0.01);
    seg.setInputCloud (cloud_filtered3);
    seg.segment (*inliers, *coefficients);
    //保存平面模型切割的PCD文件
    writer.write<pcl::PointXYZ>("out_pinmian.pcd", *cloud_filtered3, false);
    
    //欧式聚类提取
    /*pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
    seg.setOptimizeCoefficients (true);
    seg.setModelType (pcl::SACMODEL_PLANE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setMaxIterations (100);
    seg.setDistanceThreshold (0.02);
    */
    
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud (cloud_filtered2);

    std::vector<pcl::PointIndices> cluster_indices;
    pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
    ec.setClusterTolerance (0.02); // 2cm
    ec.setMinClusterSize (100);
    ec.setMaxClusterSize (25000);
    ec.setSearchMethod (tree);
    ec.setInputCloud (cloud_filtered3);
    ec.extract (cluster_indices);
    
    
    int j = 0;
   for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it)
   {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZ>);
    for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); ++pit)
    cloud_cluster->push_back ((*cloud_filtered3)[*pit]); //*
    cloud_cluster->width = cloud_cluster->size ();
    cloud_cluster->height = 1;
    cloud_cluster->is_dense = true;

    std::cout << "PointCloud representing the Cluster: " << cloud_cluster->size () << " data points." << std::endl;
	
    //保存欧式聚类后的PCD文件
    writer.write<pcl::PointXYZ> ("JuLei.pcd", *cloud_cluster, false); 
    j++;
  }
    /*cerr << "Cloud after filtering: " << endl;
    for( const auto& point: *cloud_filtered3)
    	cerr << "    " << point.x << " "
             << point.y << " "
             << point.z << std::endl;
             */
    cout << cloud_filtered3->width * cloud_filtered3->height << endl;
    cout << cloud->width * cloud->height << endl;
    
    pcl::visualization::CloudViewer viewer ("Cloud Viewer");
    viewer.showCloud (cloud_filtered3);
    
     while (!viewer.wasStopped ())
   {
   }
   
    return 0;
}
